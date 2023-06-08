#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "pch.h"

const std::string server = "localhost"; // my IP here in the local network
const int port = 8443;



void sendPostRequest(const std::string& server, int port, const std::string& endpoint, const std::string& data, const std::string& contentType) {
    httplib::SSLClient client(server, port);
    client.set_ca_cert_path("E:\\Cyber\\RootKiteRed101\\Project1\\certifacte\\server.crt");
    client.enable_server_certificate_verification(false);

    auto res = client.Post(endpoint.c_str(), data, contentType.c_str());

    if (res && res->status == 200) {
        //std::cout << "Request succeeded!" << std::endl;
        //std::cout << "Response: " << res->body << std::endl;
    }
    else {  
        //std::cerr << "Request failed." << std::endl;
        if (res) {
            //std::cerr << "Status code: " << res->status << std::endl;
        }
    }
}

std::string sendGetRequest(const std::string& server, int port, const std::string& endpoint) {
    httplib::SSLClient client(server, port);
    client.set_ca_cert_path("./server.crt");
    client.enable_server_certificate_verification(false);

    auto res = client.Get(endpoint.c_str());

    if (res && res->status == 200) {
        return res->body;
    }
    else {
        std::cerr << "Request failed." << std::endl; 
        
        if (res) {
            return res->body;
        }
    }
}

void sendMessage(const std::string& message) {
    std::string endpoint = "https://" + server + ":" + std::to_string(port) + "/";
    std::string contentType = "text/plain";

    sendPostRequest(server, port, endpoint, message, contentType);
}

std::string receiveMessages() {
    std::string endpoint = "https://" + server + ":" + std::to_string(port) + "/";

     return sendGetRequest(server, port, endpoint);
}




void downloadFile(const std::string& server, int port, const std::string& endpoint, const std::string& localDirectory) {
    httplib::SSLClient client(server, port);
    client.enable_server_certificate_verification(false);

    auto res = client.Get(endpoint.c_str());

    if (res && res->status == 200) {
        // Extract the filename from the endpoint
        std::string filename = endpoint.substr(endpoint.rfind('/') + 1);

        // Construct the local file path by appending the filename to the local directory
        std::string localFilePath = localDirectory + "/" + filename;

        std::ofstream file(localFilePath, std::ios::binary);
        if (file) {
            file.write(res->body.c_str(), res->body.size());
            file.close();
            std::cout << "File downloaded successfully: " << localFilePath << std::endl;
        }
        else {
            std::cerr << "Failed to open file for writing: " << localFilePath << std::endl;
        }
    }
    else {
        std::cerr << "Request failed." << std::endl;
        if (res) {
            std::cerr << "Status code: " << res->status << std::endl;
        }
    }
}




//int main() {
//    std::string endpoint = "https://localhost:8443/";
//    std::string data = "Hello, server!";
//    std::string contentType = "text/plain";
//    std::string message;
//
//    while (true) {
//        std::cout << "Enter a message to send to the server (or type 'exit' to close the connection): ";
//        std::getline(std::cin, message);
//
//        if (message == "exit") {
//            std::cout << "Goodbye!" << std::endl;
//            break;
//        }
//
//        sendMessage(message);
//        receiveMessages();
//    }
//
//    return 0;
//}
