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


arq = open("inscritos.csv", "r")
dic = cria_dict(arq)
arq.close()

for matricula in dic:
    print(f"Matrícula: {matricula} | Disciplinas: {dic[matricula]}\n")

print("==========================================================\n")

inv_dic = cria_dict_inverso(dic)
for d in inv_dic:
    print(f"Disciplina: {d} | Matrículas: {inv_dic[d]}\n")