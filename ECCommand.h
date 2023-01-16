//
//  ECCommand.h
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#ifndef ECCommand_h
#define ECCommand_h

#include <vector>
using namespace std;


// ******************************************************
// Implement command design pattern

class ECCommand
{
public:
    ECCommand(){}
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};

// ******************************************************
// Implement command history

class ECCommandHistory
{
public:
    ECCommandHistory(){}
    virtual ~ECCommandHistory(){}
    bool Undo()
    {
        if(!undoList.empty())
        {
            ECCommand* pop = undoList.back();
            undoList.pop_back();
            pop->UnExecute();
            redoList.push_back(pop);
        }
        else
            return 0;
        return 1;
    };
    bool Redo()
    {
        if(!redoList.empty())
        {
            ECCommand* pop = redoList.back();
            redoList.pop_back();
            undoList.push_back(pop);
            pop->Execute();
            
        }
        else
            return 0;
        return 1;
    };
    void ExecuteCmd( ECCommand *pCmd )
    {
        pCmd->Execute();
        undoList.push_back(pCmd);
        redoList.clear();
    };
    
private:
    // your code goes here
    vector<ECCommand*> undoList;
    vector<ECCommand*> redoList;
};

#endif /* ECCommand_h */
