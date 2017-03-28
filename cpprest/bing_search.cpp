//
// Created by 吴凡 on 2017/3/27.
//

#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cstdint>

web::http::client::http_client_config client_config_for_proxy() {
    web::http::client::http_client_config config;
    if(const char *env_http_proxy = std::getenv("http_proxy")) {
        web::uri proxy_uri(env_http_proxy);
        web::web_proxy proxy(proxy_uri);
        config.set_proxy(proxy);
    }
    return config;
}

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Usage: BingSearch <term> <out_file>");
        return 1;
    }

    const std::string searchTerm = argv[1];
    const std::string outputFileName = argv[2];

    auto fileBuffer = std::make_shared<concurrency::streams::streambuf<uint8_t>>();
    concurrency::streams::file_buffer<uint8_t>::open(outputFileName, std::ios::out)
            .then([=](concurrency::streams::streambuf<uint8_t> buffer) -> pplx::task<web::http::http_response> {
                *fileBuffer = buffer;

                web::http::client::http_client client("https://cn.bing.com", client_config_for_proxy());
                return client.request(web::http::methods::GET,
                                      web::http::uri_builder("/search").append_query("q", searchTerm).to_string());
            })
            .then([=](web::http::http_response response) -> pplx::task<size_t> {
                printf("Response status: %d", response.status_code());
                return response.body().read_to_end(*fileBuffer);
            })
            .then([=](size_t size) {
                return fileBuffer->close();
            })
            .wait();
    return 0;
}
