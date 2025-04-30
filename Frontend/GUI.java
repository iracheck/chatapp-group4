package com.example.client_gui;

import javafx.application.Application;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.text.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;


import java.io.IOException;


public class GUI extends Application implements Runnable {
    protected volatile String message;
    private volatile String[] clientInfo = {"-1", "-1", "-1"};
    private TextField userTextField;
    private TextField ipBox;
    private TextField portBox;
    private TextArea chatBox;

    // Run the GUI
    @Override
    public void run() {
        GUI.launch(GUI.class);
    }

    // Initial GUI (Login GUI)
    @Override
    public void start(Stage stage) throws IOException {
        stage.setTitle("Connect to server");
        // create grid pane to hold text areas/fields and labels
        GridPane gridConnect = new GridPane();
        gridConnect.setAlignment(Pos.CENTER);
        gridConnect.setHgap(10);
        gridConnect.setVgap(10);
        gridConnect.setPadding(new Insets(25, 25, 25, 25));

        // create and add title to the grid
        Text sceneTitle = new Text("Connect to server!");
        sceneTitle.setStyle("-fx-font-size: 15px; -fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(sceneTitle, 0, 0, 2, 1);

        // create and add label for display name to the grid
        Label displayName = new Label("Display Name:");
        displayName.setStyle("-fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(displayName, 0, 1);

        // create and add a text field to the grid for the user to enter display name
        userTextField = new TextField();
        userTextField.setStyle(" -fx-background-color: #5B9279; -fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(userTextField, 1, 1);

        // create and add label for ip address to the grid
        Label IP = new Label("IP Address:");
        IP.setStyle("-fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(IP, 0, 2);

        // create and add text field to the grid for the ip address
        ipBox = new TextField();
        ipBox.setStyle(" -fx-background-color: #5B9279; -fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(ipBox, 1, 2);

        // create and add label for port number to the grid
        Label Port = new Label("Port:");
        Port.setStyle("-fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(Port, 0, 3);

        // create and add text field to the grid for the port number
        portBox = new TextField();
        portBox.setStyle(" -fx-background-color: #5B9279; -fx-text-fill: white; -fx-font-family: 'Arial'");
        gridConnect.add(portBox, 1, 3);

        // submit button to accept all the information user has entered
        Button submitB = new Button("Submit");
        submitB.setStyle("-fx-background-color: #DDF2D1; -fx-text-fill: black; -fx-font-family: 'Arial'");
        submitB.setOnAction(event -> {
            String displayNameText = userTextField.getText();
            String ipAddress = ipBox.getText();
            String port = portBox.getText();

            // validate input
            if ( displayNameText.isEmpty() || ipAddress.isEmpty() || port.isEmpty() ) {
                alert("Empty Box", "Please do not leave anything blank.");
            } else {
                if (!portValidation(port)) {
                    alert("Invalid input", "Please enter a valid port number.");
                } else {
                    System.out.println("GUI: Connecting " + displayNameText + " to " + ipAddress + ":" + port);
                    try {
                        // send data to client and unpause wait
                        data.CLIENT_INFO = new String[] {userTextField.getText(), ipBox.getText(), portBox.getText()};

                        // change stages
                        chatRoom(stage);
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        });

        // add submit button to the grid
        gridConnect.add(submitB, 1, 4);
        gridConnect.setStyle("-fx-background-color: #2C4F3A;");

        Scene sceneMain = new Scene(gridConnect, 300, 275);
        stage.setScene(sceneMain);
        // stage.setResizable(false);
        stage.show();

    }

    // GUI for the chatroom
    public void chatRoom(Stage stage) throws IOException {
        // get entered info to be used
        String displayNameText = userTextField.getText();
        String ipAddress = ipBox.getText();
        String port = portBox.getText();
        stage.setTitle("Chat Room");

        BorderPane borderPane = new BorderPane();

        Label inputInfo = new Label("Display Name: " + displayNameText + " | IP Address: " + ipAddress + " | Port: " + port);
        inputInfo.setStyle("-fx-font-size: 25px; -fx-font-weight: bold; -fx-text-fill: white; -fx-font-family: 'Arial'");
        HBox infoBox = new HBox(inputInfo); // horizontal box for information
        infoBox.setStyle("-fx-background-color: #2C4F3A;");
        infoBox.setPadding(new Insets(10));
        infoBox.setAlignment(Pos.TOP_RIGHT);

        Label Users = new Label("Users");
        Users.setStyle("-fx-font-size: 20px; -fx-font-weight: bold; -fx-text-fill: white; -fx-font-family: 'Arial'");
        Label activePeople = new Label("Active People: ");
        activePeople.setStyle("-fx-font-size: 15px; -fx-text-fill: white; -fx-font-family: 'Arial'");
        // also display the active users in a new label here
        VBox usersBox = new VBox(10, Users, activePeople); // vertical box for people
        usersBox.setPadding(new Insets(35));
        usersBox.setAlignment(Pos.TOP_CENTER);

        // button to leave the chat room
        Button leaveB = new Button("Leave");
        leaveB.setStyle("-fx-font-size: 15px; -fx-background-color: #DDF2D1; -fx-text-fill: black; -fx-font-family: 'Arial'");
        HBox leaveBox = new HBox(leaveB); // horizontal box for leave button
        leaveBox.setAlignment(Pos.CENTER);
        leaveBox.setPadding(new Insets(10));
        leaveB.setOnAction(event -> {
            stage.close(); // closes the stage
        });

        // new borderpane for just the left side of the main borderpane
        // did this to have user information on top and leave button on bottom
        BorderPane leftBorderPane = new BorderPane();
        leftBorderPane.setTop(usersBox);
        leftBorderPane.setBottom(leaveBox);
        leftBorderPane.setStyle("-fx-background-color: #2C4F3A; -fx-text-fill: white; -fx-font-family: 'Arial'");

        // create text area for sent messages with a scroll pane
        //TextArea chatBox = new TextArea();
        chatBox = new TextArea();
        chatBox.setStyle("-fx-control-inner-background: #5B9279; -fx-font-size: 15px; -fx-text-fill: white; -fx-font-family: 'Arial'");
        chatBox.setEditable(false);
        chatBox.setWrapText(true);
        chatBox.setPrefHeight(400);
        chatBox.setPrefWidth(300);

        ScrollPane scrollPane = new ScrollPane(chatBox);
        scrollPane.setStyle("-fx-background-color: transparent; -fx-background: #5B9279; -fx-text-fill: white; -fx-font-family: 'Arial'");
        scrollPane.setFitToHeight(true);
        scrollPane.setFitToWidth(true);
        // Set scroll policy to ensure both horizontal and vertical scrollbars appear as needed
        scrollPane.setHbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scrollPane.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);

        // create textfield for user to send a message
        TextField sendBox = new TextField();
        sendBox.setPrefWidth(400);
        sendBox.setPromptText("Message...");
        sendBox.setStyle("-fx-font-size: 15px; -fx-background-color: #5B9279; -fx-prompt-text-fill: #3B5E4C; -fx-text-fill: white; -fx-font-family: 'Arial'");
        // button to send the message to the text area
        Button sendB = new Button("Send");
        sendB.setStyle("-fx-font-size: 15px; -fx-background-color: #DDF2D1; -fx-text-fill: black; -fx-font-family: 'Arial'");
        sendB.setOnAction(event -> {
            String message = sendBox.getText();
            data.outMessages = message;
            if (!message.isEmpty()) {
                chatBox.appendText(displayNameText + ": " + message + "\n");
                // log message
                sendBox.clear();
            }

        });

        // horizontal box to store the field for the message and send button
        HBox bottomBox = new HBox(10, sendBox, sendB);
        bottomBox.setStyle("-fx-background-color: #2C4F3A;");
        bottomBox.setPadding(new Insets(10));
        bottomBox.setAlignment(Pos.CENTER);

        // set up the border pane
        borderPane.setTop(infoBox);
        borderPane.setLeft(leftBorderPane);
        borderPane.setCenter(scrollPane);
        BorderPane.setMargin(scrollPane, new Insets( 2));
        borderPane.setBottom(bottomBox);

        // display content
        Scene sceneChat = new Scene(borderPane, 800, 500);
        stage.setScene(sceneChat);
        stage.show();
    }

    // input validation for the port
    private boolean portValidation(String port) {
        try {
            int portInt = Integer.parseInt(port);
            return portInt >= 1 && portInt <= 65535;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    // alert message
    private void alert(String title, String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Alert!!");
        alert.setHeaderText(title);
        alert.setContentText(message);
        alert.showAndWait();
    }

    // Update the text in the GUI remotely
    protected void updateGUIText() {
        try {
            //System.out.println(2);
            if (userTextField.getText() != null) {
                chatBox.appendText(userTextField.getText() + ": " + data.inMessages + "\n");
                System.out.println(userTextField.getText() + ": " + data.inMessages + "\n");

                // remove the value from inMessages
                data.inMessages = null;
            }
        } catch (Exception e) {
            //e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        launch();
    }
}