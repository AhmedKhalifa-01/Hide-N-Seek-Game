// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellHandler.h"

/*SETTERS SECTION*/

USpellHandler::USpellHandler()
{
    letterIndex = 0;
    spell = FSpellStruct();
    ss = new SpellStack();
    isSpellSet = false;
}
void USpellHandler::testSpells()
{
    UE_LOG(LogTemp, Error, TEXT("FIRST SPELL: %s"), *spellsList[1].spellWords);
}

FSpellStruct USpellHandler::getSpell() {
   // if (!spell.spellName.IsEmpty()) {
        return spell;
   // }
}

void USpellHandler::setSpell(FSpellStruct newSpell) {
    this->spell = newSpell;
}

void USpellHandler::setOwner(AActor* newOwner) {
    owner = newOwner;
}
/*
void USpellHandler::setBCastedSuccessfully(bool newBCastedSuccessfully) {
	this->bCastedSuccessfully = newBCastedSuccessfully;
}

void USpellHandler::setTarget(AActor* newTarget) {
	this->target = newTarget;
}

void USpellHandler::setLetterIndex(int newLetterIndex) {
	this->letterIndex = newLetterIndex;
}*/
ESpellCastingStatus_Enum USpellHandler::castSpell(FString letter) {
    try {
        if (ss->isEmpty()) {
            UE_LOG(LogTemp, Warning, TEXT("New Spell"));
            for (const FSpellStruct& sp : spellsList) {
                UE_LOG(LogTemp, Warning, TEXT("The word : %s"), *sp.spellWords);

                if (sp.spellWords.StartsWith(letter, ESearchCase::IgnoreCase))
                {
                    c_spell = letter;
                    ss->push(sp);
                }
                else if (sp.spellWords.Compare(letter, ESearchCase::IgnoreCase) > 0)
                {
                    break;
                }
            }
            if (ss->isEmpty()) {
                spellCastLimitFailed();
                return ESpellCastingStatus_Enum::CASTING_FAILED;
            }
        }
        else {
            c_spell += letter;
            for (int i = ss->top; i >= 0; i--) {
                FString sp = ss->spell_Stack[i].spellWords;
                if (sp.Contains(c_spell, ESearchCase::IgnoreCase)) {
                    UE_LOG(LogTemp, Warning, TEXT("Correct spell.... Continue : %s"), *sp);
                }
                else {
                    if (i == ss->top) {
                        UE_LOG(LogTemp, Warning, TEXT("Wrong spell.... Poping top immediatly: %s"), *ss->spell_Stack[ss->top].spellWords);
                        ss->pop();
                    }
                    else {
                        UE_LOG(LogTemp, Warning, TEXT("Wrong spell.... Swapping then Popping -------- Before: %s"), *ss->spell_Stack[ss->top].spellWords);
                        Swap(ss->spell_Stack[i], ss->spell_Stack[ss->top]);
                        UE_LOG(LogTemp, Warning, TEXT("Wrong spell.... Swapping then Popping -------- After: %s"), *ss->spell_Stack[ss->top].spellWords);
                        ss->pop();
                    }
                }
            }
            UE_LOG(LogTemp, Warning, TEXT("Current spell progress : %s"), *c_spell);
            if (ss->isEmpty()) {
                spellCastLimitFailed();
                return ESpellCastingStatus_Enum::CASTING_FAILED;
            }
            else if (ss->top > 0) {
                return ESpellCastingStatus_Enum::CASTING_PROGRASS;
            }
            else {
                isSpellSet = true;
                UE_LOG(LogTemp, Warning, TEXT("TOP spell : %s"), *ss->spell_Stack[ss->top].spellWords);
                spell = ss->spell_Stack[ss->top];
                if (c_spell == ss->spell_Stack[ss->top].spellWords) {
                    if (owner && spell.spellAction) {
                        spellPerformedAction = spell.spellAction->Action(owner, false);
                        return ESpellCastingStatus_Enum::CASTING_COMPLETED;
                    }
                }
                return ESpellCastingStatus_Enum::CASTING_PROGRASS;
            }
        }
        
    }
    catch (const std::exception& Exception)
    {
        UE_LOG(LogTemp, Error, TEXT("Exception occurred while casting the spell: %s"), UTF8_TO_TCHAR(Exception.what()));
    }
    return ESpellCastingStatus_Enum::CASTING_PROGRASS;
}
/*void USpellHandler::performAction() {

}*/

void USpellHandler::spellCastLimitFailed() {
    spell = FSpellStruct();
    //letterIndex = 0;
    if (ss) {
        delete ss;
    }
    ss = new SpellStack();
    c_spell = "";
    isSpellSet = false;
}

/*
***
**** Spell stack class 
***
*/

SpellStack::SpellStack() {
    top = -1;
}
void SpellStack::push(FSpellStruct spellS) {
    spell_Stack.Add(spellS);
    top++;
}
void SpellStack::pop() {
    if(!isEmpty()){
        top--;
    }
}
bool SpellStack::isEmpty() {
    if (top == -1)
        return true;
    return false;
}