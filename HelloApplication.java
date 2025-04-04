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
        //FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("submit-view.fxml"));
        //Scene scene = new Scene(fxmlLoader.load(), 320, 240);
        //Text text = new Text();
        stage.setTitle("Welcome!");
        GridPane gridConnect = new GridPane();
        gridConnect.setAlignment(Pos.CENTER);
        gridConnect.setHgap(10);
        gridConnect.setVgap(10);
        gridConnect.setPadding(new Insets(25, 25, 25, 25));

        Text sceneTitle = new Text("Connect to server!");
        gridConnect.add(sceneTitle, 0, 0, 2, 1);

        Label displayName = new Label("Display Name:");
        gridConnect.add(displayName, 0, 1);

        userTextField = new TextField();
        gridConnect.add(userTextField, 1, 1);

        Label IP = new Label("IP Addresss:");
        gridConnect.add(IP, 0, 2);

        ipBox = new TextField();
        gridConnect.add(ipBox, 1, 2);

        Label Port = new Label("Port:");
        gridConnect.add(Port, 0, 3);

        portBox = new TextField();
        gridConnect.add(portBox, 1, 3);

        Button b = new Button("Submit");
        b.setOnAction(event -> {
            String displayNameText = userTextField.getText();
            String ipAddress = ipBox.getText();
            String port = portBox.getText();

            // validate input
            if ( displayNameText.isEmpty() || ipAddress.isEmpty() || port.isEmpty() ) {
                alert("Empty Box", "Please do not leave anything blank.");// look into alerts, maybe add function
            } else {
                if (!portValidation(port)) {
                    alert("Invalid input", "Please enter a valid port number."); // another alert
                } else {
                    System.out.println("Connecting " + displayNameText + " to " + ipAddress + ":" + port);
                    try {
                        chatRoom(stage);
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        });

        gridConnect.add(b, 1, 4);

        Scene sceneMain = new Scene(gridConnect, 300, 275);
        stage.setScene(sceneMain);
        stage.show();

    }

    public void chatRoom(Stage stage) throws IOException {
        String displayNameText = userTextField.getText();
        String ipAddress = ipBox.getText();
        String port = portBox.getText();

        GridPane gridChat = new GridPane();
        stage.setTitle("Welcome to the chat room!");
        gridChat.setAlignment(Pos.TOP_RIGHT);
        gridChat.setHgap(10);
        gridChat.setVgap(10);
        gridChat.setPadding(new Insets(25, 25, 25, 25));

        Label IP = new Label("IP: " + ipAddress);
        gridChat.add(IP, 0, 0);

        Label Port = new Label("Port: " + port);
        gridChat.add(Port, 1, 0);

        Label Name = new Label("Display Name: " + displayNameText);
        gridChat.add(Name, 2, 0);

        Scene sceneChat = new Scene(gridChat, 500, 500);
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