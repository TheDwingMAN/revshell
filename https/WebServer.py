import http.server
import ssl

class MyHTTPRequestHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length)
        response = self.process_request(body.decode('utf-8'))
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.send_header('Content-Length', str(len(response)))
        self.end_headers()
        self.wfile.write(response.encode('utf-8'))

    def process_request(self, request):
        # Process the received request and generate a response
        # This is a simple example that just echoes the request message
        return "Received message: {}".format(request)

def run_server(address, port, certfile, keyfile):
    server_address = (address, port)

    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile, keyfile)

    httpd = http.server.HTTPServer(server_address, MyHTTPRequestHandler)
    httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

    print("Server is running at https://{}:{}".format(address, port))
    httpd.serve_forever()

def main():
    address = 'localhost'
    port = 8000
    certfile = 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.crt'
    keyfile = 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.key'

    run_server(address, port, certfile, keyfile)

if __name__ == '__main__':
    main()
