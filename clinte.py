import websocket
import ssl

def on_open(ws):
    print('WebSocket connection opened')
    ws.send('Hello, WebSocket Server!')

def on_message(ws, message):
    print(f'Received message: {message}')

def on_close(ws):
    print('WebSocket connection closed')

if __name__ == '__main__':
    ws_url = 'wss://localhost:8443'  # Replace with your WebSocket server URL
    ws = websocket.WebSocketApp(ws_url,
                                on_open=on_open,
                                on_message=on_message,
                                on_close=on_close,
                                )
    ws.run_forever(sslopt={"cert_reqs": ssl.CERT_NONE})
