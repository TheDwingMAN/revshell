import ssl
import http.server
import websockets
import asyncio


class WebSocketHandler:
    async def handle(self, websocket, path):
        print('WebSocket connection opened')
        try:
            async for message in websocket:
                print(f'Received message: {message}')
                await websocket.send(message)
        except websockets.exceptions.ConnectionClosedOK:
            print('WebSocket connection closed')


class MyHandler(http.server.SimpleHTTPRequestHandler):
    def handle_websocket(self):
        asyncio.ensure_future(websockets.serve(WebSocketHandler().handle, self.request, server=self.server))

    def handle_http_request(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b'Hello, HTTPS!')

    def do_GET(self):
        if self.headers.get('Upgrade') == 'websocket':
            self.handle_websocket()
        else:
            self.handle_http_request()


class HTTPSServer:
    def __init__(self, host, port, cert_file, key_file):
        self.host = host
        self.port = port
        self.cert_file = cert_file
        self.key_file = key_file

    def start(self):
        ssl_context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
        ssl_context.load_cert_chain(certfile=self.cert_file, keyfile=self.key_file)

        server_address = (self.host, self.port)
        httpd = http.server.HTTPServer(server_address, MyHandler)
        httpd.socket = ssl_context.wrap_socket(httpd.socket, server_side=True)

        print(f'Starting HTTPS server on {self.host}:{self.port}...')
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            httpd.shutdown()
            print('\nServer stopped.')


def run_https_server(host, port, cert_file, key_file):
    server = HTTPSServer(host, port, cert_file, key_file)
    server.start()


if __name__ == '__main__':
    HOST = 'localhost'
    PORT = 8443
    CERT_FILE = "E:\Cyber\RootKiteRed101\Project1\certifacte\server.crt"
    KEY_FILE = "E:\Cyber\RootKiteRed101\Project1\certifacte\server.key"

    run_https_server(HOST, PORT, CERT_FILE, KEY_FILE)
