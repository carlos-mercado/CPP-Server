# CPP-Server 🎵

A lightweight C++ HTTP server that displays your currently playing music in real-time on a web interface.

## 🌟 Features

- **Real-time Music Display**: Shows current song information from a text file
- **Cross-Network Access**: Accessible from any device on your local network
- **Polling-based Updates**: Checks for music changes every second (1000ms)
- **Simple Web Interface**: Clean HTML page displaying server info and current track
- **Non-blocking Architecture**: Server remains responsive while monitoring music changes

## 🏗️ Project Structure

```
CPP-Server/
├── server_linux.cpp          # Main server implementation
├── http_tcpServer_linux.h     # HTTP server class header
├── music.h                    # Music monitoring class header
├── notes.txt                  # Project notes
└── README.md                  # This file
```

## 🎯 How It Works

1. **Music Monitoring**: Reads current song information from `G:\now_playing\now_playing.txt`
2. **HTTP Server**: Serves a web page on port 8080 showing the current music
3. **Auto-refresh**: Updates music info every time someone visits the page
4. **Network Access**: Listens on `0.0.0.0:8080` for connections from any device

## 🚀 Quick Start

### Prerequisites

- Windows OS (uses Windows Socket API)
- g++ compiler
- A text file at `G:\now_playing\now_playing.txt` containing your current song info

### Build & Run

```bash
# Clone the repository
git clone https://github.com/carlos-mercado/CPP-Server.git
cd CPP-Server

# Compile the server
g++ -o server.exe server_linux.cpp -lws2_32

# Run the server
./server.exe
```

### Expected Output

```
*** Listening on ADDRESS: 0.0.0.0 PORT: 8080 ***

====== Waiting for a new connection ======
```

## 🌐 Access Your Server

### Local Access
- Open your browser and go to: `http://localhost:8080`

### Network Access
1. Find your computer's IP address:
   ```powershell
   ipconfig
   ```
2. Look for your Wi-Fi adapter's IPv4 address (e.g., `192.168.1.53`)
3. From any device on the same network, go to: `http://YOUR_IP:8080`

Example: `http://192.168.1.53:8080`

## 📁 Music File Setup

Create a text file at `G:\now_playing\now_playing.txt` containing your current song information:

```
Artist - Song Title
```

Example:
```
The Beatles - Hey Jude
```

The server will display "No music playing" if the file doesn't exist or is empty.

## 🔧 Configuration

### Change Port
Edit `main()` function in `server_linux.cpp`:
```cpp
http::TcpServer server("0.0.0.0", 8081);  // Change to desired port
```

### Change Music File Path
Edit the constructor in `music.h`:
```cpp
dir = L"C:\\your\\custom\\path\\music.txt";  // Change path
```

### Adjust Polling Frequency
Edit `checkAndUpdate()` in `server_linux.cpp`:
```cpp
if (timeSinceLastCheck.count() >= 500) {  // Check every 500ms instead of 1000ms
```

## 🏛️ Architecture

### Classes

- **`http::TcpServer`**: Main HTTP server handling connections and responses
- **`Music`**: Manages music file monitoring and information retrieval

### Key Components

- **Non-blocking Music Updates**: Uses time-based polling instead of blocking file system APIs
- **Dynamic Response Generation**: Rebuilds HTML response with fresh music data for each request
- **Error Handling**: Graceful handling of missing files and network errors

## 🔍 Troubleshooting

### Server Won't Start
- **Port in use**: Try a different port or stop other services using port 8080
- **Firewall**: Allow the application through Windows Firewall

### Can't Access from Other Devices
- Ensure all devices are on the same Wi-Fi network
- Check Windows Firewall settings
- Verify your IP address with `ipconfig`

### Music Not Updating
- Check if `G:\now_playing\now_playing.txt` exists
- Verify file permissions
- Check server logs for error messages

## 🛠️ Development

### Building
```bash
g++ -o server.exe server_linux.cpp -lws2_32
```

### Dependencies
- Windows Socket Library (`ws2_32`)
- Standard C++ libraries
- Windows API for file operations

## 📝 API Reference

### Endpoints

- **GET /**: Returns HTML page with current music information

### Response Format
```html
<!DOCTYPE html>
<html lang="en">
<body>
  <h1>Carlos Mercado's Server</h1>
  <p>I am currently listening to [SONG_INFO] :)</p>
</body>
</html>
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is open source. Feel free to use and modify as needed.

## 👨‍💻 Author

**Carlos Mercado** - [GitHub Profile](https://github.com/carlos-mercado)

---

*Built with ❤️ and C++*
