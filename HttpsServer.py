import http.server
import ssl

class MyHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Server is up and running')
        else:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Not found')

class MyHTTPSServer:
    def __init__(self, address, port, certfile, keyfile):
        self.server_address = (address, port)
        self.certfile = certfile
        self.keyfile = keyfile

    def start(self):
        ssl_context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
        ssl_context.load_cert_chain(certfile=self.certfile, keyfile=self.keyfile)

        httpd = http.server.HTTPServer(self.server_address, MyHandler)
        httpd.socket = ssl_context.wrap_socket(httpd.socket, server_side=True)
        
        print('Server is up and running.')
        httpd.serve_forever()

def main():
    server = MyHTTPSServer('', 8000, 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.crt', 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.key')
    server.start()

if __name__ == '__main__':
    main()
