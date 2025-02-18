import java.io.*;
import java.net.*;

public class EchoClient {

    public static void main(String[] args) {
        String serverAddress = "localhost";  // Server address (localhost for local machine)
        int serverPort = 6017;  // Server port

        try (Socket socket = new Socket(serverAddress, serverPort);
             BufferedReader inputReader = new BufferedReader(new InputStreamReader(System.in));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("Connected to echo server at " + serverAddress + ":" + serverPort);

            // Read user input and send it to the server
            String userInput;
            while ((userInput = inputReader.readLine()) != null) {
                out.println(userInput);  // Send input to the server
                System.out.println("Server echoed: " + in.readLine());  // Read response from the server
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
