package com.example.t_1;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;
import javafx.scene.layout.StackPane;


public class MainGUI extends Application {
    // Main
    public static void main(String[] args) {
        launch();
    }

    // Scrollpane
    @Override
    public void start(Stage newStage) {
        newStage.setTitle("Chat Application");

        // Create a TextArea to hold the long text document
        TextArea textArea = new TextArea();
        textArea.setWrapText(true); // Enable text wrapping

        // Add your long text here (or load it from a file)
        String longText = "Hello World!";

        textArea.setText(longText);

        // Create a ScrollPane to display the TextArea with scrollbars
        ScrollPane scrollPane = new ScrollPane(textArea);

        // Set scroll policy to ensure both horizontal and vertical scrollbars appear as needed
        scrollPane.setHbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scrollPane.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);

        // Create a StackPane to hold the ScrollPane
        StackPane root = new StackPane();
        root.getChildren().add(scrollPane);



        // Create a text field
        //Label label = new Label("a");
        TextField textField = new TextField("Enter Message Here");
        HBox textBox = new HBox();
        root.getChildren().add(textField);


        // Send button
        Button sendButton = new Button("SEND TEST");
        sendButton.setOnAction(action -> {
            System.out.println(textField.getText());
        });
        root.getChildren().add(sendButton);


        // Create a scene and add it to the stage
        root.setPadding(new Insets(20, 0, 20, 20));
        Scene scene = new Scene(root, 600, 400);
        newStage.setScene(scene);
        newStage.setMinWidth(600);
        newStage.show();
    }
}