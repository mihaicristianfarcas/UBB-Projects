import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.beans.property.SimpleStringProperty;
import Model.PrgState;
import Model.Values.Value;
import Model.Statements.IStmt;
import Model.Utils.MyIHeap;
import Model.Utils.MyIList;
import Model.Utils.MyISemaphoreTable;
import Controller.MyController;
import Model.Utils.MyIDictionary;
import Model.Utils.MyIStack;
import Model.Utils.MySemaphore;
import Model.Utils.MyStack;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.stream.Collectors;
import java.io.BufferedReader;
import Model.Values.StringValue;
import javafx.stage.Stage;

public class MainWindowController {
    private MyController controller;
    private PrgState selectedProgram;

    @FXML
    private TextField numberOfPrgStatesTextField;

    @FXML
    private TableView<Map.Entry<Integer, Value>> heapTableView;
    @FXML
    private TableColumn<Map.Entry<Integer, Value>, String> heapAddressColumn;
    @FXML
    private TableColumn<Map.Entry<Integer, Value>, String> heapValueColumn;

    @FXML
    private ListView<String> outputListView;
    @FXML
    private ListView<String> fileTableListView;
    @FXML
    private ListView<Integer> prgStateIdentifiersListView;
    @FXML
    private ListView<String> exeStackListView;

    @FXML
    private TableView<Map.Entry<String, Value>> symTableView;
    @FXML
    private TableColumn<Map.Entry<String, Value>, String> symTableVarNameColumn;
    @FXML
    private TableColumn<Map.Entry<String, Value>, String> symTableValueColumn;

    @FXML
    private TableView<Map.Entry<Integer, MySemaphore>> semaphoreTableView;
    @FXML
    private TableColumn<Map.Entry<Integer, MySemaphore>, String> semaphoreTableIndexColumn;
    @FXML
    private TableColumn<Map.Entry<Integer, MySemaphore>, String> semaphoreTableValueColumn;
    @FXML
    private TableColumn<Map.Entry<Integer, MySemaphore>, String> semaphoreTableValueListColumn;

    @FXML
    private Button runOneStepButton;

    public void setController(MyController controller) {
        this.controller = controller;
        populateAll();
    }

    @FXML
    public void initialize() {
        heapAddressColumn
                .setCellValueFactory(cellData -> new SimpleStringProperty(cellData.getValue().getKey().toString()));
        heapValueColumn
                .setCellValueFactory(cellData -> new SimpleStringProperty(cellData.getValue().getValue().toString()));

        symTableVarNameColumn.setCellValueFactory(cellData -> new SimpleStringProperty(cellData.getValue().getKey()));
        symTableValueColumn
                .setCellValueFactory(cellData -> new SimpleStringProperty(cellData.getValue().getValue().toString()));

        semaphoreTableIndexColumn
                .setCellValueFactory(cellData -> new SimpleStringProperty(cellData.getValue().getKey().toString()));
        semaphoreTableValueColumn.setCellValueFactory(
                cellData -> new SimpleStringProperty(Integer.toString(cellData.getValue().getValue().getPermits())));
        semaphoreTableValueListColumn.setCellValueFactory(
                cellData -> new SimpleStringProperty(cellData.getValue().getValue().getAcquired().toString()));

        prgStateIdentifiersListView.getSelectionModel().selectedItemProperty().addListener(
                (observable, oldValue, newValue) -> {
                    if (newValue != null) {
                        selectedProgram = controller.getPrgStates().stream()
                                .filter(p -> Objects.equals(p.getCurrentID(), newValue))
                                .findFirst()
                                .orElse(null);
                        populateExeStack();
                        populateSymTable();
                    }
                });
    }

    private void populateAll() {
        populateHeapTable();
        populateSemaphoreTable();
        populateOutput();
        populateFileTable();
        populatePrgStateIdentifiers();
        populateNumberOfPrgStates();

        if (selectedProgram == null && !controller.getPrgStates().isEmpty()) {
            selectedProgram = controller.getPrgStates().get(0);
            prgStateIdentifiersListView.getSelectionModel().select(0);
        }

        if (selectedProgram != null) {
            populateExeStack();
            populateSymTable();
        }
    }

    private void populateNumberOfPrgStates() {
        numberOfPrgStatesTextField.setText(String.valueOf(controller.getPrgStates().size()));
    }

    private void populateHeapTable() {
        MyIHeap<Value> heap = controller.getPrgStates().get(0).getHeap();
        ObservableList<Map.Entry<Integer, Value>> heapEntries = FXCollections
                .observableArrayList(heap.getContent().entrySet());
        heapTableView.setItems(heapEntries);
    }

    private void populateOutput() {
        ObservableList<String> output = FXCollections.observableArrayList();
        if (!controller.getPrgStates().isEmpty()) {
            MyIList<Value> outList = controller.getPrgStates().get(0).getOut();
            output.addAll(outList.getList().stream()
                    .map(Object::toString)
                    .collect(Collectors.toList()));
        }
        outputListView.setItems(output);
    }

    private void populateFileTable() {
        ObservableList<String> files = FXCollections.observableArrayList();
        if (!controller.getPrgStates().isEmpty()) {
            MyIDictionary<StringValue, BufferedReader> fileTable = controller.getPrgStates().get(0).getFileTable();
            files.addAll(fileTable.getContent().keySet().stream()
                    .map(StringValue::toString)
                    .toList());
        }
        fileTableListView.setItems(files);
    }

    private void populatePrgStateIdentifiers() {
        ObservableList<Integer> identifiers = FXCollections.observableArrayList();
        identifiers.addAll(controller.getPrgStates().stream()
                .map(PrgState::getCurrentID)
                .toList());
        prgStateIdentifiersListView.setItems(identifiers);
    }

    private void populateExeStack() {
        ObservableList<String> exeStack = FXCollections.observableArrayList();
        if (selectedProgram != null) {
            List<String> stackElements = new ArrayList<>();
            MyIStack<IStmt> stack = selectedProgram.getExeStack();
            MyIStack<IStmt> tempStack = new MyStack<>();

            try {
                while (!stack.isEmpty()) {
                    IStmt stmt = stack.pop();
                    stackElements.add(0, stmt.toString());
                    tempStack.push(stmt);
                }

                while (!tempStack.isEmpty()) {
                    stack.push(tempStack.pop());
                }
            } catch (Exception e) {
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setTitle("Error");
                alert.setHeaderText(null);
                alert.setContentText("Error accessing stack: " + e.getMessage());
                alert.showAndWait();
            }

            exeStack.addAll(stackElements);
        }
        exeStackListView.setItems(exeStack);
    }

    private void populateSymTable() {
        ObservableList<Map.Entry<String, Value>> symTableEntries = FXCollections.observableArrayList();
        if (selectedProgram != null) {
            MyIDictionary<String, Value> symTable = selectedProgram.getSymTable();
            List<Map.Entry<String, Value>> entries = new ArrayList<>(symTable.getContent().entrySet());
            symTableEntries.addAll(entries);

            symTableView.getItems().clear();
            symTableView.setItems(symTableEntries);

            symTableView.refresh();
        }
    }

    private void populateSemaphoreTable() {
        MyISemaphoreTable semaphoreTable = controller.getPrgStates().get(0).getSemaphoreTable();
        ObservableList<Map.Entry<Integer, MySemaphore>> semaphoreEntries = FXCollections
                .observableArrayList(semaphoreTable.getContent().entrySet());
        semaphoreTableView.setItems(semaphoreEntries);
        semaphoreTableView.refresh();
    }

    @FXML
    private void runOneStep() {
        if (controller == null) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText(null);
            alert.setContentText("No program selected!");
            alert.showAndWait();
            return;
        }

        try {
            List<PrgState> prgList = controller.removeCompletedPrg(controller.getPrgStates());
            if (prgList.isEmpty()) {
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Shutdown");
                alert.setHeaderText(null);
                alert.setContentText("Execution finished, shutting down...");
                alert.showAndWait();
                runOneStepButton.setDisable(true);
                ((Stage) runOneStepButton.getScene().getWindow()).close();
                System.exit(0);
            }

            controller.oneStepForAllPrg(prgList);
            populateAll();

        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText(null);
            alert.setContentText(e.getMessage());
            alert.showAndWait();
            runOneStepButton.setDisable(true);
            ((Stage) runOneStepButton.getScene().getWindow()).close();
            System.exit(0);
        }
    }
}