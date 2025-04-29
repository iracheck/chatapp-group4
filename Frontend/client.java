package com.example.client_gui;

import java.io.*;
import java.net.*;
import java.security.spec.ECField;
import java.util.Scanner;


public class client {
    private static boolean PAUSE = true;
    private static String SERVER_ADDRESS = null;
    private static int SERVER_PORT = -1;
    private static String USER_NAME = "Guest";

    // Sends data back to the server
    public static void send_to_server(PrintWriter server_writer) {//Sends client message to server
        //try {
            //if (data.outMessages.length() > 0) {
        server_writer.println(USER_NAME + ": " + data.outMessages);
        System.out.println(data.outMessages);
            //}
//        } catch (Exception e) {
//            //e.printStackTrace();
//        }

        // reset the message back to null
        //data.outMessages = null;

    }


    public static void main(String[] args) {
        // Create a thread that runs the GUI
        GUI comm = new GUI();
        Thread thread = new Thread(comm);
        thread.start();

        // Wait and get sever address, server port, and username
        while (PAUSE) {
            if (data.CLIENT_INFO[0] != "-1") {
                PAUSE = false;
                USER_NAME = data.CLIENT_INFO[0];
                SERVER_ADDRESS = data.CLIENT_INFO[1];
                SERVER_PORT = Integer.parseInt(data.CLIENT_INFO[2]);
            } else {
                try {
                Thread.sleep(1000);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }






//        if (args.length < 2 || args.length >= 3) {
//            SERVER_ADDRESS = "127.0.0.1";
//            SERVER_PORT = 8080;
//        }else {
//            SERVER_ADDRESS = args[0];
//            SERVER_PORT = Integer.parseInt(args[1]);
//            USER_NAME = args[2];
//        }

        // Loop until the com.example.client_gui.client successfully connects to the server
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
                    message = data.outMessages;

                    // Exit the com.example.client_gui.client if the user types "exit"
                    if ("exit".equalsIgnoreCase(message)) {
                        break;
                    }

                    // Send the message to the server
                    send_to_server(output);

                    output.println(USER_NAME + ": " + message);
                }

                // Close the socket and streams
                socket.close();
                scanner.close();
                break;  // Break out of the loop if connection is successful and com.example.client_gui.client exits

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
        // Wait for thread to end
        try {
            thread.join();
            System.out.println("GUI closed.");
        } catch(InterruptedException e) {
            e.printStackTrace();
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
                // create a new GUI object
                GUI GUIHandler = new GUI();
                String serverMessage;
                while ((serverMessage = input.readLine()) != null) {
                    // update data file
                    data.inMessages = serverMessage;

                    // Append data into GUI textbox
                    GUIHandler.updateGUIText();

                    //System.out.println("\nServer: " + serverMessage);
                }
            } catch (Exception e) {
                System.out.println("Error receiving message: " + e.getMessage());
            }
        }
    }


}