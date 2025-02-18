import java.io.*;
import java.net.*;

public class EchoServer {

    public static void main(String[] args) {
        // Define the port number where the server will listen
        int port = 6017;

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Echo server running on port " + port + "...");

            // Loop to accept multiple clients
            while (true) {
                // Accept client connections
                Socket clientSocket = serverSocket.accept();
                System.out.println("Client connected: " + clientSocket.getInetAddress());

                // Create input and output streams for communication with the client
                InputStream inputStream = clientSocket.getInputStream();
                OutputStream outputStream = clientSocket.getOutputStream();
                byte[] buffer = new byte[1024]; // Buffer to store the received data
                int bytesRead;

                // Loop to handle the data from the client
                while ((bytesRead = inputStream.read(buffer)) != -1) {
                    // Echo back the data to the client
                    outputStream.write(buffer, 0, bytesRead);
                    outputStream.flush(); // Make sure the data is sent immediately
                }

                // Client has closed the connection
                System.out.println("Client disconnected: " + clientSocket.getInetAddress());

                // Close the connection with the client
                clientSocket.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
