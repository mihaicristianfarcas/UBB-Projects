package View;

import Controller.MyController;

public class RunExampleCommand extends Command {
    private MyController controller;
    private boolean executed = false;

    public RunExampleCommand(String key, String desc, MyController ctr) {
        super(key, desc);
        this.controller = ctr;
    }

    @Override
    public void execute() {
        try {
            if(!executed) {
                controller.setDisplayFlag();
                controller.allStep();
                executed = true;
            } else {
                System.out.println("Already executed.");
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
