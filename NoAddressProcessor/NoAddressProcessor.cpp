// NoAddressProcessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>

class Memory {
private: 
    short data[1024] = {};
public:
    void write(short address, short value)
    {
        data[address] = value;
    }
    short read(short address)
    {
        return data[address];
    }
};

class Register
{
private:
    unsigned short value;

public:
    Register(short value)
    {
        this->value = value;
    }
    Register()
    {
        this->value = 0;
    }
    void incrementBy(unsigned short value)
    {
        this->value += value;
    }
    short getValue()
    {
        return this->value;
    }
    void setValue(unsigned short value)
    {
        this->value = value;
    }
};

class Stack
{
    
public:
    Register stack[8];
    int n = 8;
    int top = -1;
    int readTop()
    {
        return top;
    }
    void setTop(int tempTop)
    {
        top = tempTop;
    }
    void push(short val)
    {
        int tempTop = readTop();
        if (tempTop >= n - 1)
            return;
        else {
            tempTop++;
            setTop(tempTop);
            stack[tempTop].setValue(val);
        }
    }
    short pop() {
        int tempTop = readTop();
        if (tempTop <= -1)
            return -1;
        else {
            tempTop--;
            setTop(tempTop);
            return stack[tempTop+1].getValue();
        }
    }
    short peek() {
        if (top <= -1)
            return -1;
        else
        {
            return stack[top].getValue();
        }
    }
    void ror()
    {
        short tempVar = stack[top].getValue();
        stack[top].setValue(stack[top - 2].getValue());
        stack[top - 2].setValue(stack[top - 1].getValue());
        stack[top - 1].setValue(tempVar);
        
    }
    void rol()
    {
        short tempVar = stack[top].getValue();
        stack[top].setValue(stack[top - 1].getValue());
        stack[top - 1].setValue(stack[top - 2].getValue());
        stack[top - 2].setValue(tempVar);
    }
    void swap()
    {
        short tempVar = stack[top].getValue();
        stack[top].setValue(stack[top - 1].getValue());
        stack[top - 1].setValue(tempVar);
    }
    bool compare()
    {
        return stack[top].getValue() == stack[top - 1].getValue();
    }
    void increment(short value)
    {
        stack[top].incrementBy(value);
    }
};

void emulationStep(Stack stack, Register flag, Register command, Register counter, Memory instMemory, Memory dataMemory)
{
    std::cout << "Stack: ";
    for (int i = 0; i < 8; i++)
    {
        std::cout << stack.stack[i].getValue() << " ";
        if (i == stack.readTop())
        {
            std::cout << "] ";
        }
    }
    std::cout << std::endl << "Top at: " << stack.readTop();
    std::cout << std::endl << "Flag: " << flag.getValue() << std::endl << "Command: ";
    switch (command.getValue())
    {
    case 0b0000:
        std::cout << "PUSH " << instMemory.read(counter.getValue() + 1);
        break;
    case 0b0001:
        std::cout << "POP " << stack.peek();
        break;
    case 0b0010:
        std::cout << "READ " << stack.peek() << " TO PUSH " << dataMemory.read(stack.peek());
        break;
    case 0b0011:
        std::cout << "ADD " << stack.peek() << " AND " << stack.stack[stack.readTop() - 1].getValue();
        break;
    case 0b0100:
        std::cout << "SUB " << stack.peek() << " AND " << stack.stack[stack.readTop() - 1].getValue();
        break;
    case 0b0101:
        std::cout << "WRITE VALUE " << stack.stack[stack.readTop() - 1].getValue() << " TO ADDRESS " << stack.peek();
        break;
    case 0b0110:
        std::cout << "LDC: JUMP TO INSTRUCTION " << stack.peek();
        break;
    case 0b0111:
        std::cout << "FULL STOP";
        break;
    case 0b1000:
        std::cout << "SWAP " << stack.peek() << " AND " << stack.stack[stack.readTop() - 1].getValue();
        break;
    case 0b1001:
        std::cout << "ROR: ROTATE " << stack.peek() << ", " << stack.stack[stack.readTop() - 1].getValue() << " AND " << stack.stack[stack.readTop() - 2].getValue() << " TO THE RIGHT";
        break;
    case 0b1010:
        std::cout << "ROL: ROTATE " << stack.peek() << ", " << stack.stack[stack.readTop() - 1].getValue() << " AND " << stack.stack[stack.readTop() - 2].getValue() << " TO THE LEFT";
        break;
    case 0b1011:
        std::cout << "CMP " << stack.peek() << " AND " << stack.stack[stack.readTop() - 1].getValue();
        break;
    case 0b1100:
        std::cout << "DUP " << stack.peek();
        break;
    case 0b1101:
        std::cout << "JZ: JUMP IF ZERO FLAG POSITIVE";
        break;
    case 0b1110:
        std::cout << "INC " << stack.peek() << " BY 1";
        break;
    case 0b1111:
        std::cout << "OUT";
        break;
    }
    std::cout << std::endl << "Counter: " << counter.getValue() << std::endl;
    system("pause");
}

int CPU(Memory addedInstr, int mode)
{
    Memory dataMemory;
    Memory instMemory;
    Register pc;
    Register flags;
    Stack stackRegisters;
    Register comRegister;
    bool noStop = true;
    dataMemory.write(10, 5);
    dataMemory.write(11, 4);
    dataMemory.write(12, 3);
    dataMemory.write(13, 2);
    dataMemory.write(14, 1);
    dataMemory.write(15, 0);
    if (mode == 1)
    {
        instMemory.write(0, 0b0000); //PUSH 6 - THE SIZE OF ARRAY
        instMemory.write(1, 6);
        instMemory.write(2, 0b0000); //PUSH 10 - THE ADDRESS OF FIRST ELEMENT OF ARRAY
        instMemory.write(3, 10);
        instMemory.write(4, 0b1100); // DUP 10
        instMemory.write(5, 0b1010); // ROL
        instMemory.write(6, 0b0011); // ADD TO FIND THE END OF ARRAY
        instMemory.write(7, 0b1000); // SWAP TO PUT ADDRESS 10 ON TOP
        instMemory.write(8, 0b1100); // DUP 10 TO HAVE BOTH ADDRESS AND POINTER
        instMemory.write(9, 0b0010); // READ ADDRESS 10
        instMemory.write(10, 0b1010); // ROL TO PUT POINTER ON TOP AND ADDRESS OF LAST PRE-TOP
        instMemory.write(11, 0b1110); // INC TO NEXT ADDRESS
        instMemory.write(12, 0b1011); // CMP NEXT AND LAST ADDRESSES
        instMemory.write(13, 0b1101); // IF TRUE
        instMemory.write(14, 24); // JZ TO OUTPUT
        instMemory.write(15, 0b1001); // ROR AND SWP TO PUT NEXT ADDRESS ON TOP, SUM ON PRE-TOP
        instMemory.write(16, 0b1000);
        instMemory.write(17, 0b1100); // DUP NEXT ADDRESS
        instMemory.write(18, 0b0010); // READ ADDRESS
        instMemory.write(19, 0b1001); // ROR TO PUT TWO VALUES ON TOP
        instMemory.write(20, 0b0011); // ADD NUMBERS
        instMemory.write(21, 0b0000); // PUSH THE INSTRUCTION 10 IN STACK
        instMemory.write(22, 10);
        instMemory.write(23, 0b0110); // LDC. COUNTER IS SET TO 10. LOOP
        instMemory.write(24, 0b1001); // ROR TO PUT SUM ON TOP
        instMemory.write(25, 0b1111); // OUTPUT SUM
        instMemory.write(26, 0b0111); // FULL STOP
    }
    else
    {
        instMemory = addedInstr;
    }
    while (noStop)
    {
        comRegister.setValue(instMemory.read(pc.getValue()));
        emulationStep(stackRegisters, flags, comRegister, pc, instMemory, dataMemory);
        switch (comRegister.getValue())
        {
        case 0b0000: // PUSH
            pc.incrementBy(1);
            stackRegisters.push(instMemory.read(pc.getValue()));
            break;
        case 0b0001: // POP
            stackRegisters.pop();
            break;
        case 0b0010: // READ
            stackRegisters.push(dataMemory.read(stackRegisters.pop()));
            break;
        case 0b0011: // ADD
            stackRegisters.push(stackRegisters.pop() + stackRegisters.pop());
            break;
        case 0b0100: // SUB
            stackRegisters.push(stackRegisters.pop() - stackRegisters.pop());
            break;
        case 0b0101: // WRITE
            instMemory.write(stackRegisters.pop(), stackRegisters.pop());
            break;
        case 0b0110: // LDC
            pc.setValue(stackRegisters.pop() - 1);
            break;
        case 0b0111: // FST
            noStop = false;
            break;
        case 0b1000: // SWAP
            stackRegisters.swap();
            break;
        case 0b1001: // ROR
            stackRegisters.ror();
            break;
        case 0b1010: // ROL
            stackRegisters.rol();
            break;
        case 0b1011: //CMP
            if (stackRegisters.compare())
            {
                flags.incrementBy(1);
            }
            break;
        case 0b1100: // DUP
            stackRegisters.push(stackRegisters.peek());
            break;
        case 0b1101: // JZ
            pc.incrementBy(1);
            if (flags.getValue() == 1)
            {
                flags.setValue(0);
                stackRegisters.push(instMemory.read(pc.getValue()));
                pc.setValue(stackRegisters.pop() - 1);
            }
            break;
        case 0b1110: // INC
            stackRegisters.increment(1);
            break;
        case 0b1111: // OUT
            std::cout << "OUTPUT: " << stackRegisters.peek();
            break;
        }
        pc.incrementBy(1);
    }
    return stackRegisters.peek();
}

std::map<std::string, int> mapping()
{
    std::map<std::string, int> mapper;
    mapper["PUSH"] =  0b0000;
    mapper["POP"]  =  0b0001;
    mapper["READ"] =  0b0010;
    mapper["ADD"]  =  0b0011;
    mapper["SUB"]  =  0b0100;
    mapper["WRITE"]=  0b0101;
    mapper["LDC"]  =  0b0110;
    mapper["FST"]  =  0b0111;
    mapper["SWAP"] =  0b1000;
    mapper["ROR"]  =  0b1001;
    mapper["ROL"]  =  0b1010;
    mapper["CMP"]  =  0b1011;
    mapper["DUP"]  =  0b1100;
    mapper["JZ"]   =  0b1101;
    mapper["INC"]  =  0b1110;
    mapper["OUT"]  =  0b1111;
    return mapper;
}

Memory parser()
{
    std::ifstream f("input.txt");
    Memory instructions;
    std::map<std::string, int> mapper;
    mapper = mapping();
    int instrCnt = 0;
    bool numberWaiter = false;
    // String variable to store the read data
    std::string s;
    while (getline(f, s))
    {
            if (numberWaiter == true)
            {
                instructions.write(instrCnt, std::stoi(s));
                numberWaiter = false;
            }
            else
            {
                instructions.write(instrCnt, mapper[s]);
                if (mapper[s] == 0b0000 || mapper[s] == 0b1101) // PUSH OR JZ
                {
                    numberWaiter = true;
                }
            }
            instrCnt += 1;
    }
    f.close();
    return instructions;
}


int main()
{
    Memory parsedMem = parser();
    std::cout << "PRESS 1 FOR DIRECT MODE, 2 FOR PARSED MODE." << std::endl;
    int mode;
    std::cin >> mode;
    if (CPU(parsedMem, mode) == 15)
    {
       std::cout << "Test passed";
    }
}