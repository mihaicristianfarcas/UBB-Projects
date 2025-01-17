package View;

import Controller.MyController;

public class RunExampleCommand extends Command {
    private final MyController controller;
    private boolean executed = false;

    public RunExampleCommand(String key, String desc, MyController ctr) {
        super(key, desc);
        this.controller = ctr;
    }

    @Override
    public void execute() {
        try {
            if (!executed) {
                controller.setDisplayFlag();
                controller.allStep();
                executed = true;
            } else if (executed) {
                System.out.println("Already executed.");
            } else {
                System.out.println("Faulty execution.");
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
