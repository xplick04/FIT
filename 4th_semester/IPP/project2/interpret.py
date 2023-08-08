import sys
import re
import argparse
import xml.etree.ElementTree as ET


class Program:
    def __init__(self):
        self.input = ''
        self.sourceXML = ''

    def parse_args(self):
        parser = argparse.ArgumentParser(description='Argument parsing.')
        parser.add_argument('--input', nargs=1, help="File with XML.")
        parser.add_argument('--source', nargs=1, help="File with source.")

        args = parser.parse_args()
        if not (args.source or args.input):
            print("At least one of arguemts --source or --input must be set")
            exit(10)
        if args.source:
            self.sourceXML = args.source[0]
        if args.input:
            self.input = args.input[0]

    def get_input(self):
        return self.input

    def get_source_xml(self):
        return self.sourceXML


class Argument:
    def __init__(self, arg_type, arg_value):
        self.type = arg_type
        if arg_value is None:
            arg_value = ""
        self.value = arg_value

    def get_var_name(self):
        if not self.type == "var":
            print("Instruction argument must be variable")
            exit(32)
        splitted = self.value.split("@", 1)
        return splitted[1]

    def get_var_frame(self):
        if not self.type == "var":
            print("Instruction argument must be variable")
            exit(32)
        splitted = self.value.split("@", 1)
        return splitted[0]


class Variable:
    def __init__(self, name):
        self.name = name
        self.value = None
        self.type = None

    def __eq__(self, other):
        return self.name == other.name

    def update(self, var_value, var_type):
        self.value = var_value
        self.type = var_type


class Instruction:
    def __init__(self, child):
        if child.tag != "instruction":
            print("Wrong instruction format")
            exit(32)

        if not "order" in child.attrib.keys():
            print("Instruction order is missing")
            exit(32)
        else:
            try:
                self.order = int(child.attrib["order"])
            except:
                print("Instruction order has must be int")
                exit(32)
        if int(child.attrib["order"]) <= 0:
            print("Instruction order must be greater than zero")
            exit(32)

        if not "opcode" in child.attrib.keys():
            print("Instruction opcode is missing")
            exit(32)
        else:
            self.opcode = child.attrib["opcode"].lower()

        self.arg_count = 0
        for arg in child:
            self.arg_count += 1
            try:
                arg.attrib["type"]
            except:
                print("Instruction type is missing")
                exit(32)
            if not arg.attrib["type"] in \
                   ["bool", "string", "int", "nil", "label", "type", "var"]:
                print("Instruction type is not in valid format")
                exit(32)
            if arg.tag == "arg1":
                self.arg1 = Argument(arg.attrib["type"], arg.text)
            elif arg.tag == "arg2":
                self.arg2 = Argument(arg.attrib["type"], arg.text)
            elif arg.tag == "arg3":
                self.arg3 = Argument(arg.attrib["type"], arg.text)
            else:
                print("Wrong instruction argument format")
                exit(32)

    def check_num_of_args(self):
        # no arguments
        if self.opcode in \
                ["createframe", "pushframe", "popframe", "return", "break"]:
            if not self.arg_count == 0:
                print("Instruction " + self.opcode + " does not have zero arguments")
                exit(32)
        # 1 argument
        elif self.opcode in \
                ["defvar", "call", "pushs", "pops", "write", "label", "jump", "exit", "dprint"]:
            if not self.arg_count == 1:
                print("Instruction " + self.opcode + " does not have one argument")
                exit(32)
            if not hasattr(self, 'arg1'):
                print("Instruction " + self.opcode + " does not have arg1")
                exit(32)
        # 2 arguments
        elif self.opcode in \
                ["move", "not", "int2char", "read", "strlen", "type"]:
            if not self.arg_count == 2:
                print("Instruction " + self.opcode + " does not have two arguments")
                exit(32)
            if not hasattr(self, 'arg1'):
                print("Instruction " + self.opcode + " does not have arg1")
                exit(32)
            if not hasattr(self, 'arg2'):
                print("Instruction " + self.opcode + " does not have arg2")
                exit(32)
        # 3 arguments
        elif self.opcode in \
                ["add", "sub", "mul", "idiv", "lt", "gt", "eq", "and", "or", "stri2int",
                 "concat", "getchar", "setchar", "jumpifeq", "jumpifneq"]:
            if not self.arg_count == 3:
                print("Instruction " + self.opcode + " does not have three arguments")
                exit(32)
            if not hasattr(self, 'arg1'):
                print("Instruction " + self.opcode + " does not have arg1")
                exit(32)
            if not hasattr(self, 'arg2'):
                print("Instruction " + self.opcode + " does not have arg2")
                exit(32)
            if not hasattr(self, 'arg3'):
                print("Instruction " + self.opcode + " does not have arg3")
                exit(32)
        else:
            print("Unknown opcode")
            exit(32)

    def check_for_duplicates(self, instructions):
        for instruction in instructions:
            if instruction.order == self.order:
                print("Duplicate order in instructions")
                exit(32)


class Labels:
    def __init__(self):
        self.label_dictionary = {}

    def add(self, label_name, label_order):
        if not self.label_dictionary:
            self.label_dictionary[label_name] = label_order
        else:
            if label_name in self.label_dictionary:
                print("Label name already exists")
                exit(52)
            else:
                self.label_dictionary[label_name] = label_order

    def get_position(self, label_name, instructions):
        if label_name not in self.label_dictionary:
            print("Label name does not exist")
            exit(52)
        for instruction in instructions:
            if instruction.opcode == "label":
                if instruction.arg1.value == label_name:
                    return instructions.index(instruction)


class Frame:
    def __init__(self):
        self.vars = []

    def add(self, var_name):
        variable = Variable(var_name)
        for var in self.vars:
            if var == variable:
                print("Redefinition of variable " + var_name)
                exit(52)
        self.vars.append(variable)

    def get_value(self, var_name):
        for var in self.vars:
            if var.name == var_name:
                return var.value
        print("Acces to undefined variable")
        exit(54)

    def get_type(self, var_name):
        for var in self.vars:
            if var.name == var_name:
                if var.type is None:
                    print("Variable type is missing")
                    exit(56)
                return var.type
        print("Acces to undefined variable")
        exit(54)

    def update_var(self, var_name, var_value, var_type):
        if var_type not in ["bool", "string", "int", "nil", "label", "type", "var"]:
            print("Wrong variable type")
            exit(53)
        for var in self.vars:
            if var.name == var_name:
                var.update(var_value, var_type)
                return
        print("Acces to undefined variable")
        exit(54)


class Frames:
    def __init__(self):
        self.GF = Frame()   # global frame init
        self.LF = []    # local frame stack init
        self.TF = None

    def defvar(self, var_name, var_frame):
        if var_frame == "GF":
            self.GF.add(var_name)
        elif var_frame == "LF":
            if not self.LF:
                print("Local frame was not initialized yet")
                exit(55)
            self.LF[-1].add(var_name)
        elif var_frame == "TF":
            if not self.TF:
                print("Temporary frame was not initialized yet")
                exit(55)
            self.TF.add(var_name)
        else:
            print("variable frame has wrong format")
            exit(32)

    def create_frame(self):
        self.TF = Frame()

    def push_frame(self):
        if not self.TF:
            print("Temporary frame does not exists")
            exit(55)
        self.LF.append(self.TF)
        self.TF = None

    def pop_frame(self):
        if not self.LF:
            print("Local frame does not exists")
            exit(55)
        self.TF = self.LF.pop()

    def get_symb_value(self, arg):
        if arg.type == "var":
            if re.match("^GF@", arg.value):
                return self.GF.get_value(arg.value[3:])
            elif re.match("^LF@", arg.value):
                if not self.LF:
                    print("Local frame does not exists")
                    exit(55)
                return self.LF[-1].get_value(arg.value[3:])
            elif re.match("^TF@", arg.value):
                if not self.TF:
                    print("Temporary frame does not exists")
                    exit(55)
                return self.TF.get_value(arg.value[3:])

        elif arg.type == "string":
            return arg.value
        elif arg.type == "int":
            try:
                return int(arg.value)
            except:
                print("Wrong value " + arg.value + " in int"  )
                exit(53)
        elif arg.type == "bool":
            if arg.value == "true":
                return True
            elif arg.value == "false":
                return False
            else:
                print("Wrong value " + arg.value + " in bool"  )
                exit(53)
        elif arg.type == "nil":
            if arg.value == "nil":
                return None
            else:
                print("Wrong value " + arg.value + " in nil")
                exit(53)

    def get_symb_type(self, arg):
        if arg.type == "var":
            if re.match("^GF@", arg.value):
                return self.GF.get_type(arg.value[3:])
            elif re.match("^LF@", arg.value):
                if not self.LF:
                    print("Local frame does not exists")
                    exit(55)
                return self.LF[-1].get_type(arg.value[3:])
            elif re.match("^TF@", arg.value):
                if not self.TF:
                    print("Temporary frame does not exists")
                    exit(55)
                return self.TF.get_type(arg.value[3:])
        else:
            return arg.type

    def update_var(self, var_name, var_frame, var_value, var_type):
        if var_frame == "GF":
            self.GF.update_var(var_name, var_value, var_type)
        elif var_frame == "LF":
            if not self.LF:
                print("Local frame does not exists")
                exit(55)
            self.LF[-1].update_var(var_name, var_value, var_type)
        elif var_frame == "TF":
            if not self.TF:
                print("Temporary frame does not exists")
                exit(55)
            self.TF.update_var(var_name, var_value, var_type)

    def get_result(self, instruction):
        symb_value1 = self.get_symb_value(instruction.arg2)
        symb_type1 = self.get_symb_type(instruction.arg2)

        if instruction.arg_count == 3:
            symb_value2 = self.get_symb_value(instruction.arg3)
            symb_type2 = self.get_symb_type(instruction.arg3)

        # specific error checking
        if instruction.opcode in ["add", "sub", "mul", "idiv"]:
            if not (symb_type1 == symb_type2 == "int"):
                print("Both operands in arithmetic operations must be type of integer")
                exit(53)

        elif instruction.opcode in ["lt", "gt"]:
            if (not symb_type1 == symb_type2) or symb_type1 == "nil":
                print("Both operands in LT/GT operations must be of same type and can not be nil")
                exit(53)

        elif instruction.opcode in ["eq", "jumpifeq", "jumpifneq"]:
            if (not symb_type1 == "nil") and (not symb_type2 == "nil"):
                if not symb_type1 == symb_type2:
                    print("Both operands in EQ/JUMPIFEQ/JUMPIFNEQ operations must be of same type or at least one must be nil")
                    exit(53)

        elif instruction.opcode in ["and", "or"]:
            if not (symb_type1 == symb_type2 == "bool"):
                print("Both operands in logic operations must be type of boolean")
                exit(53)

        elif instruction.opcode == "not":
            if not (symb_type1 == "bool"):
                print("Operand in logic operation must be type of boolean")
                exit(53)

        elif instruction.opcode == "int2char":
            if not (symb_type1 == "int"):
                print("Operand in int2char operation must be type of integer")
                exit(53)

        elif instruction.opcode in ["stri2int", "getchar"]:
            if not (symb_type1 == "string"):
                print("First perand in stri2int operation must be type of string")
                exit(53)
            if not (symb_type2 == "int"):
                print("Second operand in stri2int operation must be type of integer")
                exit(53)
            if not (0 <= symb_value2 < len(symb_value1)):
                print("Wrong index position in instruction stri2int")
                exit(58)

        elif instruction.opcode == "concat":
            if not (symb_type1 == symb_type2 == "string"):
                print("Both operands in concat operation must be type of string")
                exit(53)

        elif instruction.opcode == "strlen":
            if not (symb_type1 == "string"):
                print("Operand in strlen operation must be type of string")
                exit(53)

        # returning computed result
        if instruction.opcode == "add":
            return [symb_value1 + symb_value2, "int"]
        elif instruction.opcode == "sub":
            return [symb_value1 - symb_value2, "int"]
        elif instruction.opcode == "mul":
            return [symb_value1 * symb_value2, "int"]
        elif instruction.opcode == "idiv":
            if symb_value2 == 0:
                print("Can not divide by zero")
                exit(57)
            else:
                return [int(symb_value1 / symb_value2), "int"]
        elif instruction.opcode == "lt":
            return [symb_value1 < symb_value2, "bool"]
        elif instruction.opcode == "gt":
            return [symb_value1 > symb_value2, "bool"]
        elif instruction.opcode == "eq":
            return [symb_value1 == symb_value2, "bool"]
        elif instruction.opcode == "and":
            return [symb_value1 and symb_value2, "bool"]
        elif instruction.opcode == "or":
            return [symb_value1 or symb_value2, "bool"]
        elif instruction.opcode == "not":
            return [not symb_value1, "bool"]
        elif instruction.opcode == "int2char":
            try:
                return [chr(symb_value1), "string"]
            except:
                print("Value is not valid ordinal value of char in Unicode")
                exit(58)
        elif instruction.opcode == "stri2int":
            return [ord(symb_value1[symb_value2]), "int"]
        elif instruction.opcode == "concat":
            return [symb_value1 + symb_value2, "string"]
        elif instruction.opcode == "strlen":
            return [len(symb_value1), "int"]
        elif instruction.opcode == "getchar":
            return [symb_value1[symb_value2], "string"]
        elif instruction.opcode == "type":
            if symb_type1 is None:
                symb_type1 = ""
            return [symb_type1, "string"]
        elif instruction.opcode == "jumpifeq":
            return symb_value1 == symb_value2
        elif instruction.opcode == "jumpifneq":
            return symb_value1 != symb_value2


def main():
    program = Program()  # creating new instance of program
    program.parse_args()  # check and assign of program arguments

    if not program.get_source_xml() == '':  # if source file was set
        try:  # checking file acces
            source = open(program.get_source_xml(), 'r')
        except:
            print("file " + program.get_source_xml() + " can not be opened")
            exit(11)
    else:
        source = sys.stdin

    if not program.get_input() == '':  # if input file was set
        try:  # checking file acces
            input = open(program.get_input(), 'r')
        except:
            print("file " + program.get_input() + " can not be opened")
            exit(11)
    else:
        input = sys.stdin

    if not source.readline() in \
           ["<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"]:
        print("Header missing in source file")
        exit(32)

    try:
        XMLTree = ET.parse(source)
        root = XMLTree.getroot()
    except:
        print("XML file is not well-formed")
        exit(31)

    for atribute in root.attrib:
        if atribute not in ["language", "name", "description"]:
            print("XML header atribute unrecognised")
            exit(32)

    if not (root.tag == "program" and root.attrib["language"].lower() == "ippcode22"):
        print("XML program atribute is not valid")
        exit(32)

    instructions = []   # instructions array init
    labels = Labels()  # labels init

    # first iteration for loading all instructions into array
    for child in root:
        instruction = Instruction(child)
        instruction.check_num_of_args()
        instruction.check_for_duplicates(instructions)
        instructions.append(instruction)
        if instruction.opcode == "label":
            labels.add(instruction.arg1.value, instruction.order)

    # sorts instruction by their order
    instructions.sort(key=lambda x: x.order)

    instruction_counter = 0
    frames = Frames()   # Frames and GF Frame init
    data_stack = call_stack = []    # stack for data and stack for calls init

    while instruction_counter < len(instructions):  # main cycle
        instruction = instructions[instruction_counter]
        if instruction.opcode == "defvar":
            var_name = instruction.arg1.get_var_name()
            var_frame = instruction.arg1.get_var_frame()
            frames.defvar(var_name, var_frame)
        elif instruction.opcode == "createframe":
            frames.create_frame()
        elif instruction.opcode == "pushframe":
            frames.push_frame()
        elif instruction.opcode == "popframe":
            frames.pop_frame()
        elif instruction.opcode == "move":
            var_name = instruction.arg1.get_var_name()
            var_frame = instruction.arg1.get_var_frame()
            symb_value = frames.get_symb_value(instruction.arg2)
            symb_type = frames.get_symb_type(instruction.arg2)
            frames.update_var(var_name, var_frame, symb_value, symb_type)
        elif instruction.opcode == "write":
            symb_value = frames.get_symb_value(instruction.arg1)
            symb_type = frames.get_symb_type(instruction.arg1)
            if symb_type in ["string", "int"]:
                print(symb_value, end="")
            elif symb_type == "nil":
                print("")
            elif symb_type == "bool":
                if symb_value:
                    print("true")
                else:
                    print("false")
            else:
                print("Wrong type in write")
                exit(53)
        elif instruction.opcode == "pushs":
            symb_value = frames.get_symb_value(instruction.arg1)
            symb_type = frames.get_symb_type(instruction.arg1)
            data_stack.append([symb_value, symb_type])
        elif instruction.opcode == "pops":
            var_name = instruction.arg1.get_var_name()
            var_frame = instruction.arg1.get_var_frame()
            if not len(data_stack):
                print("Data stack is empty")
                exit(56)
            tmp = data_stack.pop()
            frames.update_var(var_name, var_frame, tmp[0], tmp[1])
        elif instruction.opcode in \
                ["add", "sub", "mul", "idiv", "lt", "gt", "eq", "and",
                 "or", "not", "int2char", "stri2int", "concat", "getchar", "strlen", "type"]:
            var_name = instruction.arg1.get_var_name()
            var_frame = instruction.arg1.get_var_frame()
            result = frames.get_result(instruction)
            frames.update_var(var_name, var_frame, result[0], result[1])
        elif instruction.opcode == "read":
            var_name = instruction.arg1.get_var_name()
            var_frame = instruction.arg1.get_var_frame()
            var_type = instruction.arg2.value
            value = input.readline()
            if not value:
                frames.update_var(var_name, var_frame, None, "nil")
            if var_type == "int":
                try:
                    frames.update_var(var_name, var_frame, int(value), "int")
                except:
                    frames.update_var(var_name, var_frame, None, "nil")
            elif var_type == "string":
                frames.update_var(var_name, var_frame, value, "string")
            elif var_type == "bool":
                value = value.lower()
                if value == "true":
                    frames.update_var(var_name, var_frame, True, "bool")
                else:
                    frames.update_var(var_name, var_frame, False, "bool")
            else:
                print("Unknown variable type in read")
                exit(53)
        elif instruction.opcode == "setchar":
            var_name = instruction.arg1.get_var_name()
            var_frame = instruction.arg1.get_var_frame()
            symb_value1 = frames.get_symb_value(instruction.arg1)
            symb_type1 = frames.get_symb_type(instruction.arg1)
            symb_value2 = frames.get_symb_value(instruction.arg2)
            symb_type2 = frames.get_symb_type(instruction.arg2)
            symb_value3 = frames.get_symb_value(instruction.arg3)
            symb_type3 = frames.get_symb_type(instruction.arg3)
            if not (symb_type1 == "string"):
                print("First perand in setchar operation must be type of string")
                exit(53)
            if not (symb_type2 == "int"):
                print("Second operand in setchar operation must be type of integer")
                exit(53)
            if not (symb_type3 == "string"):
                print("Third operand in setchar operation must be type of string")
                exit(53)
            if not (0 <= symb_value2 < len(symb_value1)):
                print("Wrong index position in operation setchar")
                exit(58)
            if not symb_value3:
                print("Empty string in operation setchar")
                exit(58)
            symb_value1 = list(symb_value1)
            symb_value1[symb_value2] = symb_value3[0]
            symb_value1 = "".join(symb_value1)
            frames.update_var(var_name, var_frame, symb_value1, "string")
        elif instruction.opcode == "exit":
            symb_value1 = frames.get_symb_value(instruction.arg1)
            symb_type1 = frames.get_symb_type(instruction.arg1)
            if not symb_type1 == "int":
                print("Wrong operand type in exit instruction")
                exit(53)
            if not (0 <= symb_value1 < 50):
                print("Wrong operand value in exit instruction")
                exit(57)
            exit(symb_value1)
        elif instruction.opcode == "jump":
            instruction_counter = labels.get_position(instruction.arg1.value, instructions)
        elif instruction.opcode in ["jumpifeq", "jumpifneq"]:
            if frames.get_result(instruction):
                instruction_counter = labels.get_position(instruction.arg1.value, instructions)
        elif instruction.opcode == "call":
            call_stack.append(instruction_counter)
            instruction_counter = labels.get_position(instruction.arg1.value, instructions)
        elif instruction.opcode == "return":
            if not len(call_stack):
                print("Call stack is empty")
                exit(56)
            instruction_counter = call_stack.pop()
        elif instruction.opcode in ["dprint", "break", "label"]:
            pass
        else:
            print("Unknown opcode")
            exit(32)

        instruction_counter += 1

    exit(0)

if __name__ == '__main__':
    main()
