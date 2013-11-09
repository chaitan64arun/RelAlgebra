#ifndef RAEXPR_H
#define RAEXPR_H
//#include "cross_product.h"
#include<iostream>
#include<string.h>
#include <sstream>
#include <fstream>

using namespace std;

class Strt {
public:
    char reln[100];
    Strt(char* str) {
        strcpy(reln, str);
    }
};

class PL {
public:
    char* attr;
    PL* next;

    PL* update(PL** Root, char* value) {
        PL* temp = *Root;
        PL* New = new PL;
        char* tmp;
        tmp = new char(sizeof (value));
        strcpy(tmp, value);
        New->attr = tmp;
        New->next = NULL;
        if (*Root == NULL) {
            *Root = New;
        } else {
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = New;
        }
        return *Root;
    }
};

class Relation {
    ifstream myReadFile;
    ofstream fileout;
    int count;
public:
    char fnam[100];

    Relation(const char* p) {
        strcpy(fnam, p);
    }

    Relation(Relation* p) {
        strcpy(fnam, p->fnam);
    }

    void openscan() {
        char* datfile;
        datfile = new char();
        strcpy(datfile, this->fnam);
        strcat(datfile, ".dat");
        myReadFile.open(datfile);
    }

    void closescan() {
        myReadFile.close();
    }

    void closeout() {
        fileout.close();
    }

    string nextrecord() {
        string output;
        while (!myReadFile.eof()) {
            getline(myReadFile, output);
            return output;
        }
    }

    int no_of_records() {
        string line, str;
        count = 0;
        char* datfile;
        datfile = new char();
        strcpy(datfile, this->fnam);
        strcat(datfile, ".dat");
        myReadFile.open(datfile);
        while (getline(myReadFile, line)) {
            count++;
        }
        myReadFile.close();
        return count;
    }

    int openprint() {
        fileout.open(fnam);
        return 1;
    }

    void printtofile(string toprint) {
        fileout << toprint << endl;

    }
};

class RAExpr {
public:

    virtual char *evaluate() {
    }
};

class BaseExpr : public RAExpr {
public:
    char reln[100];

    BaseExpr(char* str) {
        strcpy(reln, str);
    }

    char* evaluate() {

        return reln;
    }
};

/*class Bioper: public RAExpr{
        public:
        Relation *input1,*input2;
        Bioper(Relation *i1,Relation *i2){
                input1=i1;
                input2=i2;
        }
        virtual Relation *evaluate(){}//
};
 */
class Cross : public RAExpr {
    int input1_total, input2_total;
    int count1, count2;

public:
    Relation *input1, *input2;
    string output, temp1, temp2;
    char* outputfile;

    Cross(RAExpr* i1, RAExpr* i2) {
        input1 = new Relation(i1->evaluate());
        input2 = new Relation(i2->evaluate());
    }

    char *evaluate() {
        input1_total = input1->no_of_records();
        input2_total = input2->no_of_records();
        count1 = 0;
        count2 = 0;

        char outfile[20];
        static int index;
        index++;
        sprintf(outfile, "cross%d", index);
        outputfile = new char(sizeof (outfile));
        strcpy(outputfile, outfile);
        sprintf(outfile, "cross%d.dat", index);
        Relation outrel(outfile);
        outrel.openprint();
        //fileout<<"Arun";
        input1->openscan();
        while (count1 < input1_total) {
            temp1 = input1->nextrecord();
            count2 = 0;
            input2->openscan();
            while (count2 < input2_total) {
                temp2 = input2->nextrecord();
                if (count1 == 0 && count2 == 0) {
                    output = temp1 + "|" + temp2;
                    outrel.printtofile(output);
                }
                if (count1 != 0 && count2 != 0) {
                    output = temp1 + "|" + temp2;
                    outrel.printtofile(output);
                }
                count2++;
            }
            input2->closescan();
            count1++;
        }
        input1->closescan();
        outrel.closeout();
        return outputfile;
    }
};

class Minus : public RAExpr {
    int input1_total, input2_total;
    int count1, count2;

public:
    char* outputfile;
    Relation *input1, *input2;
    string output, temp1, temp2;

    Minus(RAExpr* i1, RAExpr* i2) {
        input1 = new Relation(i1->evaluate());
        input2 = new Relation(i2->evaluate());
    }

    char *evaluate() {
        input1_total = input1->no_of_records();
        input2_total = input2->no_of_records();
        count1 = 0;
        count2 = 0;
        int flag = 0;

        char outfile[20];
        static int index;
        index++;
        sprintf(outfile, "minus%d", index);
        outputfile = new char(sizeof (outfile));
        strcpy(outputfile, outfile);
        sprintf(outfile, "minus%d.dat", index);
        Relation outrel(outfile);
        outrel.openprint();
        input1->openscan();
        while (count1 < input1_total) {
            temp1 = input1->nextrecord();
            count2 = 0;
            input2->openscan();
            flag = 0;

            while (count2 < input2_total) {
                temp2 = input2->nextrecord();

                if (count1 == 0 && count2 == 0) {
                    if (temp1 == temp2) {
                        output = temp1;
                        outrel.printtofile(output);
                    } else break;
                }
                if (count1 != 0 && count2 != 0 && temp1 == temp2) {
                    output = temp1;
                    flag = 1;
                }
                count2++;
            }
            if (flag == 0 && count1 != 0)
                outrel.printtofile(temp1);
            input2->closescan();
            count1++;
        }
        input1->closescan();
        outrel.closeout();
        return outputfile;
    }
};

class Union : public RAExpr {
    int input1_total, input2_total;
    int count1, count2;

public:
    Relation *input1, *input2;
    string output, temp1, temp2;
    char* outputfile;

    Union(RAExpr* i1, RAExpr* i2) {
        input1 = new Relation(i1->evaluate());
        input2 = new Relation(i2->evaluate());
    }

    char *evaluate() {
        input1_total = input1->no_of_records();
        input2_total = input2->no_of_records();
        count1 = 0;
        count2 = 0;
        int flag = 0;

        char outfile[20];
        static int index;
        index++;
        sprintf(outfile, "union%d", index);
        outputfile = new char(sizeof (outfile));
        strcpy(outputfile, outfile);
        sprintf(outfile, "union%d.dat", index);
        Relation outrel(outfile);
        outrel.openprint();
        input1->openscan();
        while (count1 < input1_total) {
            temp1 = input1->nextrecord();
            count2 = 0;
            input2->openscan();
            flag = 0;

            while (count2 < input2_total) {
                temp2 = input2->nextrecord();
                if (count1 == 0 && count2 == 0) {
                    if (temp1 == temp2) {
                        output = temp1;
                        outrel.printtofile(output);
                    } else {
                        input1->closescan();
                        input2->closescan();
                        outrel.closeout();
                        return input1->fnam;
                    }
                }
                if (count1 != 0 && count2 != 0) {
                    if (temp1 == temp2)
                        flag = 1;
                }
                count2++;
            }
            if (flag == 0 && count1 != 0)
                outrel.printtofile(temp1);
            input2->closescan();
            count1++;
        }
        input1->closescan();
        count2 = 0;
        input2->openscan();
        temp2 = input2->nextrecord();
        while (count2 < input2_total - 1) {
            temp2 = input2->nextrecord();
            outrel.printtofile(temp2);
            count2++;
        }
        input2->closescan();
        outrel.closeout();
        return outputfile;
    }
};

class Pi : public RAExpr {
    int input1_total, count1, partnum, linenum;
    char* outputfile;
public:
    Relation *input1;
    PL* plist;

    Pi(PL* hd, RAExpr* rel) {
        input1 = new Relation(rel->evaluate());
        plist = hd;
    }

    char *evaluate() {

        string temprec1, temprec2, part, subpart, insrec, insrec2;
        input1_total = input1->no_of_records();
        int ds = 0;
        char outfile[20];

        static int index;
        index++;
        sprintf(outfile, "pi%d", index);
        outputfile = new char(sizeof (outfile));
        strcpy(outputfile, outfile);
        sprintf(outfile, "pi%d.dat", index);
        Relation outrel(outfile);
        outrel.openprint();
        input1->openscan();
        linenum = 0;
        int count[5];
        int itr = 0;
        while (count1 <= input1_total) {
            linenum++;
            temprec2 = input1->nextrecord();
            istringstream linestream(temprec2);
            partnum = 0;
            while (linenum == 1 && plist != NULL) {
                temprec1 = plist->attr;
                if (plist != NULL)
                    plist = plist->next;
                partnum = 0;
                while (getline(linestream, part, '|')) {
                    partnum++;
                    istringstream linestream2(part);
                    getline(linestream2, subpart, ':');
                    if (subpart == temprec1) {
                        if (ds == 0)
                            insrec = insrec + part;
                        else
                            insrec = insrec + "|" + part;
                        ds++;
                        count[itr] = partnum;
                        itr++;
                        if (plist != NULL) {
                            temprec1 = plist->attr;
                            plist = plist->next;
                        }
                    }
                }
                outrel.printtofile(insrec);
                count1++;
                linenum++;
            }
            itr = 0;
            ds = 0;
            insrec = "";
            while (getline(linestream, part, '|')) {
                partnum++;
                while (partnum == count[itr]) {
                    if (ds == 0)
                        insrec = insrec + part;
                    else
                        insrec = insrec + "|" + part;
                    ds++;
                    itr++;
                }
            }
            if (insrec != "")
                outrel.printtofile(insrec);
            count1++;
        }
        outrel.closescan();
        input1->closescan();
        plist = NULL;


        return outputfile;
    }
};

class Rho : public RAExpr {
    int input1_total;
    char* outputfile;

public:
    char outfile[20];
    Relation *input1;
    PL* plist;

    Rho(Strt* newname, PL* hd, RAExpr* rel) {
        input1 = new Relation(rel->evaluate());
        plist = hd;
        strcpy(outfile, newname->reln);
    }

    char *evaluate() {
        //
        string temprec1, temprec2, part, subpart, insrec;
        input1_total = input1->no_of_records();
        int ds = 0, linenum = 0, count = 0;

        outputfile = new char(sizeof (outfile));
        strcpy(outputfile, outfile);
        strcat(outfile, ".dat");
        Relation outrel(outfile);
        outrel.openprint();
        input1->openscan();
        temprec2 = input1->nextrecord();
        while (count == 0 && plist != NULL) {
            istringstream linestream(temprec2);
            while (getline(linestream, part, '|')) {
                temprec1 = plist->attr;
                istringstream linestream2(part);
                while (getline(linestream2, subpart, ':')) {
                }
                if (ds == 0)
                    insrec = temprec1 + ":" + subpart;
                else insrec = insrec + "|" + temprec1 + ":" + subpart;
                ds++;
                if (plist != NULL)
                    plist = plist->next;
            }
            if (insrec != "")
                outrel.printtofile(insrec);
        }
        input1_total--;
        while (input1_total != 0) {
            temprec2 = input1->nextrecord();
            outrel.printtofile(temprec2);
            input1_total--;
        }
        outrel.closescan();
        input1->closescan();
        plist = NULL;

        return outputfile;
    }
};
#endif
