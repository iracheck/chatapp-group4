package org.example.chatroom;

import javafx.application.Application;
import javafx.css.PseudoClass;
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
                        chatRoom(stage);
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        });

        gridConnect.add(submitB, 1, 4);

        Scene sceneMain = new Scene(gridConnect, 300, 275);
        stage.setScene(sceneMain);
        stage.show();

    }

    public void chatRoom(Stage stage) throws IOException {
        String displayNameText = userTextField.getText();
        String ipAddress = ipBox.getText();
        String port = portBox.getText();
        stage.setTitle("Welcome to the chat room!");

        BorderPane borderPane = new BorderPane();

        Label inputInfo = new Label("Display Name: " + displayNameText + " | IP Address: " + ipAddress + " | Port: " + port);
        HBox infoBox = new HBox(inputInfo);
        infoBox.setPadding(new Insets(10));
        infoBox.setAlignment(Pos.TOP_RIGHT);

        Label Users = new Label("Users");
        Label activePeople = new Label("Active People: ");
        //also display the active users in a new label here
        VBox usersBox = new VBox(Users, activePeople);
        usersBox.setPadding(new Insets(25));
        usersBox.setAlignment(Pos.TOP_CENTER);

        TextArea chatBox = new TextArea();
        chatBox.setEditable(false);
        chatBox.setWrapText(true);
        chatBox.setStyle("-fx-background-color: white;");

        TextField sendBox = new TextField();
        sendBox.setPromptText("Message...");
        Button sendB = new Button("Send");
        sendB.setOnAction(event -> {

        });

        PseudoClass empty = PseudoClass.getPseudoClass("empty");
        sendBox.pseudoClassStateChanged(empty, true);
        sendBox.textProperty().addListener((obs, oldText, newText) -> {
            sendBox.pseudoClassStateChanged(empty, newText.isEmpty());
        });


        borderPane.setTop(infoBox);
        borderPane.setLeft(usersBox);
        borderPane.setCenter(chatBox);
        borderPane.setBottom(sendBox);

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
