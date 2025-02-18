import java.io.*;
import java.util.concurrent.*;

public class pipes {

    public static void main(String[] args) {
        // Create pipes
        PipedInputStream pipeInput1 = new PipedInputStream();
        PipedOutputStream pipeOutput1 = new PipedOutputStream();
        PipedInputStream pipeInput2 = new PipedInputStream();
        PipedOutputStream pipeOutput2 = new PipedOutputStream();

        try {
            // Connect the pipes
            pipeOutput1.connect(pipeInput1);  // Connect first output to first input
            pipeOutput2.connect(pipeInput2);  // Connect second output to second input

            // Create threads for the two processes
            Thread senderThread = new Thread(new Sender(pipeOutput1, pipeInput2));
            Thread receiverThread = new Thread(new Receiver(pipeInput1, pipeOutput2));

            // Start the threads
            senderThread.start();
            receiverThread.start();

            // Wait for both threads to finish
            senderThread.join();
            receiverThread.join();

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }

    // Sender thread: Sends the original message
    static class Sender implements Runnable {
        private PipedOutputStream outputStream;
        private PipedInputStream inputStream;

        public Sender(PipedOutputStream outputStream, PipedInputStream inputStream) {
            this.outputStream = outputStream;
            this.inputStream = inputStream;
        }

        @Override
        public void run() {
            try {
                // Original message
                String message = "Hi Mohit Gujarti";

                // Write the original message to the pipe
                outputStream.write(message.getBytes());
                outputStream.flush();
                outputStream.close(); // Close the pipe after sending the message

                // Read the modified message from the second process
                byte[] modifiedMessage = new byte[1024];
                int bytesRead = inputStream.read(modifiedMessage);
                System.out.println("Modified message: " + new String(modifiedMessage, 0, bytesRead));
                inputStream.close();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    // Receiver thread: Reverses the case of each character
    static class Receiver implements Runnable {
        private PipedInputStream inputStream;
        private PipedOutputStream outputStream;

        public Receiver(PipedInputStream inputStream, PipedOutputStream outputStream) {
            this.inputStream = inputStream;
            this.outputStream = outputStream;
        }

        @Override
        public void run() {
            try {
                // Read the original message from the first process
                byte[] originalMessage = new byte[1024];
                int bytesRead = inputStream.read(originalMessage);

                // Reverse the case of each character in the message
                StringBuilder reversedMessage = new StringBuilder();
                for (int i = 0; i < bytesRead; i++) {
                    char c = (char) originalMessage[i];
                    if (Character.isLowerCase(c)) {
                        reversedMessage.append(Character.toUpperCase(c));
                    } else if (Character.isUpperCase(c)) {
                        reversedMessage.append(Character.toLowerCase(c));
                    } else {
                        reversedMessage.append(c); // Non-alphabet characters remain the same
                    }
                }

                // Send the modified message back through the second pipe
                outputStream.write(reversedMessage.toString().getBytes());
                outputStream.flush();
                outputStream.close(); // Close the pipe after sending the modified message

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
