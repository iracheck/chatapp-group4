import java.io.*;
import java.net.*;
import java.util.Scanner;

public class client {

    private static String SERVER_ADDRESS = null;
    private static int SERVER_PORT = -1;
   
    public static void main(String[] args) {
        
        if (args.length < 2 || args.length >= 3) {
          SERVER_ADDRESS = "127.0.0.1";
          SERVER_PORT = 8080;     
        }else {
          SERVER_ADDRESS = args[0];
          SERVER_PORT = Integer.parseInt(args[1]);
        }
        // Loop until the client successfully connects to the server
        while (true) {
            try (Socket socket = new Socket()) {
                // Try to connect to the server with a timeout
                socket.connect(new InetSocketAddress(SERVER_ADDRESS, SERVER_PORT), 5000);  // 5-second timeout

                System.out.println("Connected to server.");

                // Create a Scanner for user input
                Scanner scanner = new Scanner(System.in);

                // Create input and output streams
                PrintWriter output = new PrintWriter(socket.getOutputStream(), true);
                BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                // Create and start a thread to receive messages
                Thread receiveThread = new Thread(new ReceiveMessages(input));
                receiveThread.start();

                // Send messages to the server
                String message;
                while (true) {
                    // Read input from the user
                    message = scanner.nextLine();

                    // Exit the client if the user types "exit"
                    if ("exit".equalsIgnoreCase(message)) {
                        break;
                    }

                    // Send the message to the server
                    output.println(message);
                }

                // Close the socket and streams
                socket.close();
                scanner.close();
                break;  // Break out of the loop if connection is successful and client exits

            } catch (IOException e) {
                System.out.println("Failed to connect to the server. Retrying in 10 seconds...");
                try {
                    Thread.sleep(10000);  // Wait for 10 seconds before retrying
                } catch (InterruptedException ex) {
                    System.out.println("Retry interrupted: " + ex.getMessage());
                    break;
                }
            }
        }
    }

    // Runnable class to handle receiving messages from the server
    static class ReceiveMessages implements Runnable {
        private BufferedReader input;

        public ReceiveMessages(BufferedReader input) {
            this.input = input;
        }

        @Override
        public void run() {
            try {
                String serverMessage;
                while ((serverMessage = input.readLine()) != null) {
                    System.out.println("\nServer: " + serverMessage);
                }
            } catch (IOException e) {
                System.out.println("Error receiving message: " + e.getMessage());
            }
        }
    }
}
