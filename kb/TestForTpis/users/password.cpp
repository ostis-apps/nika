#include <openssl/bio.h>
#include <openssl/evp.h>
#include <iostream>
#include <string>

std::string base64_encode(const std::string& input) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new(BIO_s_mem());
    BIO_push(b64, bio);

    BIO_write(b64, input.c_str(), input.length());
    BIO_flush(b64);

    char* buffer;
    long size = BIO_get_mem_data(bio, &buffer);
    std::string encoded(buffer, size);

    BIO_free_all(b64);

    return encoded;
}

std::string base64_decode(const std::string& input) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new_mem_buf(input.c_str(), input.length());
    BIO_push(b64, bio);

    char* buffer = new char[input.length()];
    BIO_read(b64, buffer, input.length());

    std::string decoded(buffer);
    delete[] buffer;

    BIO_free_all(b64);

    return decoded;
}

int main() {
    std::string password = "MyPassword123";

    std::string encodedPassword = base64_encode(password);
    std::cout << "Encoded Password: " << encodedPassword << std::endl;

    std::string decodedPassword = base64_decode(encodedPassword);
    std::cout << "Decoded Password: " << decodedPassword << std::endl;

    return 0;
}