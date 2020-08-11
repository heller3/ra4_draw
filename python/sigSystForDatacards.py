#Code to append signal systematics to datacards

#Hardcoded items that may need edits:
    #dir_datacards
    #f_sigSys
    #variations

import os
import pandas as pd

#datacard directory
dir_datacards = "../statistics/testdir/datacards/"
list_datacards = os.listdir(dir_datacards)
list_datacards.sort(key=lambda x: (x,len(x)))

#open the signal systematics txt file
f_sigSys = open("../signalSystematics.txt", "r")
variations = ["mChi",
              "mLSP",
              "Nominal",
              "lepSFup",
              "lepSFdown",
              "L1prefireUp",
              "L1prefireDown",
              "ISRUp",
              "ISRDown",
              "q2Up",
              "q2Down",
              "btagHFUp",
              "btagHFDown",
              "btagLFUp",
              "btagLFDown",
              "btagFSUp",
              "btagFSDown",
              "puUp",
              "puDown",
              "JESup",
              "JESdown",
              "JERup",
              "JERdown",
              #"genMET",
              "FatJet_genPt",
              "FastSimMETRed",
              "TriggerEff"]

#Code to format variation title to match datacards
for iVariation in range(len(variations)):
    variations[iVariation]=variations[iVariation].lower()
    if len(variations[iVariation])<23:
        nSpaces = 17-len(variations[iVariation])
        variations[iVariation] = variations[iVariation]+(" "*nSpaces)+"lnN   "

#Code to properly order the list of datacards.
#This will match the way the signal systematics file is written.
listofcards = []
for i in range(len(list_datacards)):
    mystr = list_datacards[i]
    iLSP = mystr.index("-",16)
    iChi = mystr.index("mChi-",0,16)
    a = mystr[iLSP:][1:-6]
    if len(a)==1:
        b = mystr[:iLSP]+mystr[iLSP:][:1]+'00'+mystr[iLSP:][1:]
    elif len(a)==2:
        b = mystr[:iLSP]+mystr[iLSP:][:1]+'0'+mystr[iLSP:][1:]
    else:
        b = mystr
    listofcards.append((mystr,b))

listofcards.sort(key = lambda x: x[1])

datacards = []
for i in range(len(list_datacards)):
    datacards.append(listofcards[i][0])
print("sorted datacards by mChi then by mLSP...")

#parse the signal systematics file into dataframe
df_sigSys = pd.read_csv( "../signalSystematics.txt", sep = " ", header = None )
df_sigSys.columns = variations
df_sigSys = df_sigSys.transpose()
df_sigSys = df_sigSys.round(3)
print("parsed signal systematics...")

#open the datacard and append
for iFile in range(len(datacards)):
#for iFile in range(1):
    f = open(dir_datacards+datacards[iFile],"a")
    print("opened "+dir_datacards+datacards[iFile])

    linesAdded = 0

    #now, we want to design the line to append
    for iRow in range(3,len(variations)):
        combLine = False
        noSkipLine = True

        var_string = variations[iRow]
        upIndex = var_string.find("up")
        downIndex = var_string.find("down")

        if(upIndex == -1 and downIndex == -1):
            line = variations[iRow]
        elif(upIndex == -1 and downIndex != -1):
            noSkipLine = False
        else:
            line = var_string[:upIndex]+"  "+var_string[upIndex:][2:]
            combLine = True

        upperLim = 12*(iFile+1)
        lowerLim = upperLim-12
        
        if bool(noSkipLine):
            for iColumn in range(lowerLim,upperLim):
                if not bool(combLine):
                    value = str(df_sigSys[iColumn][iRow])
                else:
                    value = str(df_sigSys[iColumn][iRow+1])+"/"+str(df_sigSys[iColumn][iRow])

                if len(value) < 17:
                    nSpaces = 17 - len(value)
                    value = value + (" " * nSpaces)
                if iColumn == upperLim:
                    line = line + value
                else:
                    line = line + value + 3*("-                ")
            linesAdded = linesAdded + 1
        else:
            continue
        f.write(line+"\n")
    f.close()

    f_read = open(dir_datacards+datacards[iFile],"r")
    linelist = f_read.readlines()
    nuisanceLine = linelist[2]
    nuisanceIdx = nuisanceLine.index(" number of nuisance parameters")
    parameters = int(nuisanceLine[:nuisanceIdx][5:])
    parameters = str(parameters+linesAdded)
    linelist[2] = "kmax " + parameters + " number of nuisance parameters\n"
    f_read.close()

    f_write = open(dir_datacards+datacards[iFile],"w")
    f_write.writelines(linelist)
    f_write.close()
    print("closed "+dir_datacards+datacards[iFile])

