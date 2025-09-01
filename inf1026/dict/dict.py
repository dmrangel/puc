def cria_dict(arq):
    dic = dict()
    lmatriculas = []
    for line in arq:
        materias = []
        dados = line.strip().split(";")
        matricula = dados[0]
        materias = dados[1:]
        lmatriculas.append(matricula)
        dic[matricula] = materias
    return dic, lmatriculas

arq = open("inscritos.csv", "r")
dic, lmatriculas = cria_dict(arq)

for matricula in lmatriculas:
    print(f"Matrícula: {matricula} | Disciplinas: {dic[matricula]}\n")
