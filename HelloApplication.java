package org.example.chatroom;

import javafx.application.Application;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.text.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;


import java.io.IOException;

public class HelloApplication extends Application {
    private TextField userTextField;
    private TextField ipBox;
    private TextField portBox;
    @Override
    public void start(Stage stage) throws IOException {
        stage.setTitle("Welcome!");
        // create grid pane to hold text areas/fields and labels
        GridPane gridConnect = new GridPane();
        gridConnect.setAlignment(Pos.CENTER);
        gridConnect.setHgap(10);
        gridConnect.setVgap(10);
        gridConnect.setPadding(new Insets(25, 25, 25, 25));

        // create and add title to the grid
        Text sceneTitle = new Text("Connect to server!");
        gridConnect.add(sceneTitle, 0, 0, 2, 1);

        // create and add label for display name to the grid
        Label displayName = new Label("Display Name:");
        gridConnect.add(displayName, 0, 1);

        // create and add a text field to the gird for the user to enter display name
        userTextField = new TextField();
        gridConnect.add(userTextField, 1, 1);

        // create and add label for ip address to the grid
        Label IP = new Label("IP Address:");
        gridConnect.add(IP, 0, 2);

        // create and add text field to the grid for the ip address
        ipBox = new TextField();
        gridConnect.add(ipBox, 1, 2);

        // create and add label for port number to the grid
        Label Port = new Label("Port:");
        gridConnect.add(Port, 0, 3);

        // create and add text field to the grid for the port number
        portBox = new TextField();
        gridConnect.add(portBox, 1, 3);

        // submit button to accept all the information user has entered
        Button submitB = new Button("Submit");
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
                    System.out.println("Connecting " + displayNameText + " to " + ipAddress + ":" + port);
                    try {
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

        Scene sceneMain = new Scene(gridConnect, 300, 275);
        stage.setScene(sceneMain);
        stage.show();

    }

    public void chatRoom(Stage stage) throws IOException {
        // get entered info to be used
        String displayNameText = userTextField.getText();
        String ipAddress = ipBox.getText();
        String port = portBox.getText();
        stage.setTitle("Welcome to the chat room!");

        BorderPane borderPane = new BorderPane();

        Label inputInfo = new Label("Display Name: " + displayNameText + " | IP Address: " + ipAddress + " | Port: " + port);
        HBox infoBox = new HBox(inputInfo); // horizontal box for information
        infoBox.setPadding(new Insets(10));
        infoBox.setAlignment(Pos.TOP_RIGHT);

        Label Users = new Label("Users");
        Label activePeople = new Label("Active People: ");
        //also display the active users in a new label here
        VBox usersBox = new VBox(10, Users, activePeople); // vertical box for people
        usersBox.setPadding(new Insets(25));
        usersBox.setAlignment(Pos.TOP_CENTER);

        // button to leave the chat room
        Button leaveB = new Button("Leave");
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

        // create text area for sent messages with a scroll pane
        TextArea chatBox = new TextArea();
        chatBox.setEditable(false);
        chatBox.setWrapText(true);
        chatBox.setStyle("-fx-background-color: white;");
        chatBox.setPrefHeight(400);
        chatBox.setPrefWidth(300);

        ScrollPane scrollPane = new ScrollPane(chatBox);
        scrollPane.setFitToHeight(true);
        scrollPane.setFitToWidth(true);
        // Set scroll policy to ensure both horizontal and vertical scrollbars appear as needed
        scrollPane.setHbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scrollPane.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);

        // create textfield for user to send a message
        TextField sendBox = new TextField();
        sendBox.setPrefWidth(400);
        sendBox.setPromptText("Message...");
        // button to send the message to the text area
        Button sendB = new Button("Send");
        sendB.setOnAction(event -> {
            String message = sendBox.getText();
            if (!message.isEmpty()) {
                chatBox.appendText(displayNameText + ": " + message + "\n");
                // log message
                sendBox.clear();
            }
        });

        // horizontal box to store the field for the message and send button
        HBox bottomBox = new HBox(10, sendBox, sendB);
        bottomBox.setPadding(new Insets(10));
        bottomBox.setAlignment(Pos.CENTER);

        // set up the border pane
        borderPane.setTop(infoBox);
        borderPane.setLeft(leftBorderPane);
        borderPane.setCenter(scrollPane);
        borderPane.setBottom(bottomBox);

        Scene sceneChat = new Scene(borderPane, 500, 500);
        stage.setScene(sceneChat);
        stage.show();


    }

    private boolean portValidation(String port) {
        try {
            int portInt = Integer.parseInt(port);
            return portInt >= 1 && portInt <= 65535;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    private void alert(String title, String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Alert!!");
        alert.setHeaderText(title);
        alert.setContentText(message);
        alert.showAndWait();
    }
    public static void main(String[] args) {
        launch();
    }
}
