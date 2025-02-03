import Model.Statements.IStmt;
import Model.PrgState;
import Model.Types.Type;
import Model.Utils.MyIDictionary;
import Model.Utils.MyDictionary;
import Repository.MyFileRepository;
import Repository.MyIRepository;
import Controller.MyController;
import View.HardcodedPrograms;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ListView;
import javafx.stage.Stage;
import java.util.List;

public class ProgramsWindowController {
    @FXML
    private ListView<String> programsListView;
    List<IStmt> programs = HardcodedPrograms.getPrograms();

    @FXML
    public void initialize() {
        ObservableList<String> programStrings = FXCollections.observableArrayList();
        for (IStmt stmt : programs) {
            MyIDictionary<String, Type> typeEnv = new MyDictionary<>();
            try {
                stmt.typeCheck(typeEnv);
                programStrings.add(stmt.toString());
            } catch (Exception e) {
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setTitle("Error");
                alert.setHeaderText(null);
                alert.setContentText("Error loading program " + stmt.toString() + " : " + e.getMessage());
                alert.showAndWait();
            }
        }

        programsListView.setItems(programStrings);
    }

    @FXML
    private void executeProgram() {
        String selectedProgram = programsListView.getSelectionModel().getSelectedItem();
        if (selectedProgram == null) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText(null);
            alert.setContentText("No program selected!");
            alert.showAndWait();
            return;
        }

        int index = programsListView.getSelectionModel().getSelectedIndex();
        IStmt program = programs.get(index);

        try {
            Model.PrgState prgState = new PrgState(program);
            MyIRepository repository = new MyFileRepository("log" + (index + 1) + ".txt");
            repository.addProgram(prgState);
            MyController controller = new MyController(repository);
            // TODO change this
            // controller.setDisplayFlag();

            FXMLLoader loader = new FXMLLoader(getClass().getResource("mainWindow.fxml"));
            Parent root = loader.load();
            MainWindowController mainWindowController = loader.getController();
            mainWindowController.setController(controller);

            Stage stage = new Stage();
            stage.setTitle("Program Execution");
            stage.setScene(new Scene(root, 800, 600));
            stage.show();

            ((Stage) programsListView.getScene().getWindow()).close();

        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText(null);
            alert.setContentText("Error creating program state: " + e.getMessage());
            alert.showAndWait();
        }
    }
}