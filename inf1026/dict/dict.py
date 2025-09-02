def cria_dict(arq):
    dic = dict()
    for line in arq:
        materias = []
        dados = line.strip().split(";")
        matricula = dados[0]
        materias = dados[1:]
        dic[matricula] = materias
    return dic

def cria_dict_inverso(dic):
    inv_dic = dict()
    ldisciplinas = []
    for matricula in dic:
        for d in dic[matricula]:
            if d not in ldisciplinas:
                inv_dic[d] = [matricula]
                ldisciplinas.append(d)
            else:
                inv_dic[d].append(matricula)
    return inv_dic

def att_dic_inv_dic_with_list(dic, inv_dic, lista):
    novodic, inv_novodic = dic, inv_dic
    for matricula in novodic:
        if matricula in lista:
            novodic[matricula] = None
    for d in inv_novodic:
        for matricula in inv_novodic[d]:
            if matricula in lista:
                inv_novodic[d].remove(matricula)
    return novodic, inv_novodic

def att_dic_inv_dic_with_dict(dic, inv_dic, ndic):
    novodic, inv_novodic = dic, inv_dic
    for matricula in novodic:
        for d in novodic[matricula]:
            if d in ndic[matricula]:
                novodic[matricula].remove[d]
    for d in inv_novodic:
        for matricula in ndic:
            if matricula in inv_novodic[d]:
                inv_novodic[d].remove(matricula)
    return novodic, inv_novodic



arq = open("inscritos.csv", "r")
dic = cria_dict(arq)
arq.close()

print("1.1 DICIONÁRIO NORMAL:")
for matricula in dic:
    print(f"Matrícula: {matricula} | Disciplinas: {dic[matricula]}\n")

print("==========================================================\n")
print("1.2 DICIONÁRIO INVERSO:")

inv_dic = cria_dict_inverso(dic)
for d in inv_dic:
    print(f"Disciplina: {d} | Matrículas: {inv_dic[d]}\n")

print("==========================================================\n")
print("1.3 DICIONÁRIOS ATUALIZADOS\n")

ltrancaram = ['1612299', '1713300']
print("POR LISTA:")
print("DICIONÁRIO NORMAL")
novodic, inv_novodic = att_dic_inv_dic_with_list(dic, inv_dic, ltrancaram)
for matricula in novodic:
    print(f"Matrícula: {matricula} | Disciplinas: {novodic[matricula]}\n")
print("DICIONÁRIO INVERSO")
for d in novodic:
    print(f"Disciplina: {d} | Matrículas: {inv_novodic[d]}\n")

print("==========================================================\n")

dic_tranc = {'1612299':['ENG1000', 'FIS1033'], '1713300':['ENG1000', 'FIS1033']}
print("POR DICIONÁRIO:")
print("DICIONÁRIO NORMAL")
novodic2, inv_novodic2 = att_dic_inv_dic_with_dict(dic, inv_dic, dic_tranc)
for matricula in novodic2:
    print(f"Matrícula: {matricula} | Disciplinas: {novodic2[matricula]}\n")
print("DICIONÁRIO INVERSO")
for d in inv_novodic2:
    print(f"Disciplina: {d} | Matrículas: {inv_novodic2[d]}\n")
