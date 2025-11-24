# Generated from antlr/IoTiny.g4 by ANTLR 4.13.2
from antlr4 import *
if "." in __name__:
    from .IoTinyParser import IoTinyParser
else:
    from IoTinyParser import IoTinyParser

# This class defines a complete generic visitor for a parse tree produced by IoTinyParser.

class IoTinyVisitor(ParseTreeVisitor):

    # Visit a parse tree produced by IoTinyParser#ProgramRule.
    def visitProgramRule(self, ctx:IoTinyParser.ProgramRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementListNonEmpty.
    def visitStatementListNonEmpty(self, ctx:IoTinyParser.StatementListNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementListEmpty.
    def visitStatementListEmpty(self, ctx:IoTinyParser.StatementListEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementListener.
    def visitStatementListener(self, ctx:IoTinyParser.StatementListenerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementTrigger.
    def visitStatementTrigger(self, ctx:IoTinyParser.StatementTriggerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementSendCommand.
    def visitStatementSendCommand(self, ctx:IoTinyParser.StatementSendCommandContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementAssignmentOrCall.
    def visitStatementAssignmentOrCall(self, ctx:IoTinyParser.StatementAssignmentOrCallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementIf.
    def visitStatementIf(self, ctx:IoTinyParser.StatementIfContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementFor.
    def visitStatementFor(self, ctx:IoTinyParser.StatementForContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#StatementFunction.
    def visitStatementFunction(self, ctx:IoTinyParser.StatementFunctionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ListenerDefinitionRule.
    def visitListenerDefinitionRule(self, ctx:IoTinyParser.ListenerDefinitionRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#TriggerCallRule.
    def visitTriggerCallRule(self, ctx:IoTinyParser.TriggerCallRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#SendCommandRule.
    def visitSendCommandRule(self, ctx:IoTinyParser.SendCommandRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#AssignmentOrCallRule.
    def visitAssignmentOrCallRule(self, ctx:IoTinyParser.AssignmentOrCallRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#AssignmentTailAssign.
    def visitAssignmentTailAssign(self, ctx:IoTinyParser.AssignmentTailAssignContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#AssignmentTailCall.
    def visitAssignmentTailCall(self, ctx:IoTinyParser.AssignmentTailCallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FunctionDefinitionRule.
    def visitFunctionDefinitionRule(self, ctx:IoTinyParser.FunctionDefinitionRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ParameterListNonEmpty.
    def visitParameterListNonEmpty(self, ctx:IoTinyParser.ParameterListNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ParameterListEmpty.
    def visitParameterListEmpty(self, ctx:IoTinyParser.ParameterListEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ParameterListTailNonEmpty.
    def visitParameterListTailNonEmpty(self, ctx:IoTinyParser.ParameterListTailNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ParameterListTailEmpty.
    def visitParameterListTailEmpty(self, ctx:IoTinyParser.ParameterListTailEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#IfStatementRule.
    def visitIfStatementRule(self, ctx:IoTinyParser.IfStatementRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ForLoopRule.
    def visitForLoopRule(self, ctx:IoTinyParser.ForLoopRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#BlockRule.
    def visitBlockRule(self, ctx:IoTinyParser.BlockRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ConditionRule.
    def visitConditionRule(self, ctx:IoTinyParser.ConditionRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#RelopGt.
    def visitRelopGt(self, ctx:IoTinyParser.RelopGtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#RelopLt.
    def visitRelopLt(self, ctx:IoTinyParser.RelopLtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#RelopEq.
    def visitRelopEq(self, ctx:IoTinyParser.RelopEqContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#RelopNeq.
    def visitRelopNeq(self, ctx:IoTinyParser.RelopNeqContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#RelopGte.
    def visitRelopGte(self, ctx:IoTinyParser.RelopGteContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#RelopLte.
    def visitRelopLte(self, ctx:IoTinyParser.RelopLteContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ExpressionRule.
    def visitExpressionRule(self, ctx:IoTinyParser.ExpressionRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ExpressionTailNonEmpty.
    def visitExpressionTailNonEmpty(self, ctx:IoTinyParser.ExpressionTailNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ExpressionTailEmpty.
    def visitExpressionTailEmpty(self, ctx:IoTinyParser.ExpressionTailEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#TermRule.
    def visitTermRule(self, ctx:IoTinyParser.TermRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#TermTailNonEmpty.
    def visitTermTailNonEmpty(self, ctx:IoTinyParser.TermTailNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#TermTailEmpty.
    def visitTermTailEmpty(self, ctx:IoTinyParser.TermTailEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorInteger.
    def visitFactorInteger(self, ctx:IoTinyParser.FactorIntegerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorString.
    def visitFactorString(self, ctx:IoTinyParser.FactorStringContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorIdentifier.
    def visitFactorIdentifier(self, ctx:IoTinyParser.FactorIdentifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorList.
    def visitFactorList(self, ctx:IoTinyParser.FactorListContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorParenthesized.
    def visitFactorParenthesized(self, ctx:IoTinyParser.FactorParenthesizedContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorCall.
    def visitFactorCall(self, ctx:IoTinyParser.FactorCallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#FactorNoCall.
    def visitFactorNoCall(self, ctx:IoTinyParser.FactorNoCallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ListLiteralRule.
    def visitListLiteralRule(self, ctx:IoTinyParser.ListLiteralRuleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ListItemsNonEmpty.
    def visitListItemsNonEmpty(self, ctx:IoTinyParser.ListItemsNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ListItemsEmpty.
    def visitListItemsEmpty(self, ctx:IoTinyParser.ListItemsEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ListItemsTailNonEmpty.
    def visitListItemsTailNonEmpty(self, ctx:IoTinyParser.ListItemsTailNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ListItemsTailEmpty.
    def visitListItemsTailEmpty(self, ctx:IoTinyParser.ListItemsTailEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ArgumentListNonEmpty.
    def visitArgumentListNonEmpty(self, ctx:IoTinyParser.ArgumentListNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ArgumentListEmpty.
    def visitArgumentListEmpty(self, ctx:IoTinyParser.ArgumentListEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ArgumentListTailNonEmpty.
    def visitArgumentListTailNonEmpty(self, ctx:IoTinyParser.ArgumentListTailNonEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#ArgumentListTailEmpty.
    def visitArgumentListTailEmpty(self, ctx:IoTinyParser.ArgumentListTailEmptyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#AddOpPlus.
    def visitAddOpPlus(self, ctx:IoTinyParser.AddOpPlusContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#AddOpMinus.
    def visitAddOpMinus(self, ctx:IoTinyParser.AddOpMinusContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#MulOpMul.
    def visitMulOpMul(self, ctx:IoTinyParser.MulOpMulContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#MulOpDiv.
    def visitMulOpDiv(self, ctx:IoTinyParser.MulOpDivContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by IoTinyParser#MulOpMod.
    def visitMulOpMod(self, ctx:IoTinyParser.MulOpModContext):
        return self.visitChildren(ctx)



del IoTinyParser