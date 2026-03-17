# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

mydict= {'Huguinho':[1.20,45],
         'Luisinho':[1.10,60],
         'Zezinho':[1,100],
         'Patinhas':[1.10,40],
         'Donald':[1.2,50]}

# print(type(mydict))
# print(mydict)

def mostreIMCxChave(meuDictParametro):
    for k,v in meuDictParametro.items():
        imc = v[1]/(v[0]**2)
        print('%s tem o IMC de %f' %(k,imc))
        

# mostreIMCxChave(mydict)

def pesoMedio(meuDict):
    total_peso = 0
    for v in meuDict.values():
        total_peso += v[1]
    return total_peso/len(meuDict)

print("O peso medio é de %2.f" %pesoMedio(mydict))


def atualizarPesoDict(meuDict, pesoMedio):
    x = 0.1 * pesoMedio 
    
    for k in meuDict.keys():
        if 'u' in k:
            valor = meuDict[k]
            valor[1] = valor[1] + x
            
            # meuDict[k][1] = 0.1*pesoMedio + meuDict[k][1]


# print("Printando o dicionario original")
# print(mydict)

# p = pesoMedio(mydict)
# atualizarPesoDict(mydict, p)

# print("Printando dicionario atualizado")
# print(mydict)


lista_valores = list(mydict.values())
print(type(lista_valores))
print(lista_valores)

print(list(mydict.keys()))
























