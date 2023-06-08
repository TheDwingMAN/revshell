import ssl
from http.server import BaseHTTPRequestHandler, HTTPServer
import os
import pygame
import sys

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        response_data = input("Enter response for client: ")
        response_length = len(response_data)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.send_header('Content-length', str(response_length))
        self.end_headers()
        self.wfile.write(response_data.encode())
        
        #print(f"Send to client: {response_data}")

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b'Received POST data: ' + post_data)
        
        print(post_data.decode())

# Initialize Pygame
pygame.init()

# Set up the window
win_width, win_height = 800, 600
win = pygame.display.set_mode((win_width, win_height))
pygame.display.set_caption("HTTP Server App")

# Set up the buttons
button_width, button_height = 200, 100
button_x, button_y = 300, 200
button_color = (0, 255, 0)  # Green

# Function to handle button click
def handle_button_click():
    message = input("Enter response for client: ")
    sendMessage(message)

def sendMessage(message):
    endpoint = "https://" + HOST + ":" + str(PORT) + "/"
    content_length = len(message)
    
    client = ssl.wrap_socket(socket.socket(socket.AF_INET, socket.SOCK_STREAM), ssl_version=ssl.PROTOCOL_TLS)
    client.connect((HOST, PORT))
    request = f"POST {endpoint} HTTP/1.1\r\nContent-Type: text/html\r\nContent-Length: {content_length}\r\n\r\n{message}"
    client.send(request.encode())
    response = client.recv(4096).decode()
    print(response)

# Run the game loop
running = True
clock = pygame.time.Clock()

while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            mouse_pos = pygame.mouse.get_pos()
            if pygame.Rect(button_x, button_y, button_width, button_height).collidepoint(mouse_pos):
                handle_button_click()

    # Draw the window and button
    win.fill((255, 255, 255))  # Fill the window with white color
    pygame.draw.rect(win, button_color, (button_x, button_y, button_width, button_height))

    # Update the display
    pygame.display.flip()

    # Limit the frame rate
    clock.tick(60)

# Quit the game
pygame.quit()
sys.exit()
