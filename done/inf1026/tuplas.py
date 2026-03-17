def get_session(sessao, tss):
    for s in tss:
        if s[0] == sessao:
             return s
    print("Nenhuma sessão com esse ID")

def get_id(s):
    return s[0]

def get_name(s):
    return s[1]

def get_custo(s):
     return s[2]

def get_horaminuto(s):
    hora = s[3]//60
    minuto = s[3]%60
    return hora, minuto

def get_dias(s):
     return s[4]

def get_maior_menor(tss):
    maior = (1, tss[0][2])
    menor = (1, tss[0][2])
    for s in tss:
        id = get_id(s)
        custo = get_custo(s)
        if custo < menor[1]:
            menor = id, custo
        elif custo > maior[1]:
            maior = id, custo
    return menor, maior

def get_mais_dias(tss):
    maior = (1, len(tss[0][4]))
    for s in tss:
        id = get_id(s)
        dias = len(get_dias(s))
        if dias > maior[1]:
            maior = id, dias
    return maior

tss = ((1, 'universo infinito', 100, 120, ('segunda', 'quarta')), (15, 'astros', 90, 60, ('segunda', 'quarta')), (13,'O sol', 70, 45, ('sexta',)), (4,'Extraterrestres', 100, 100, ('terça',)), (25,'Area45', 50, 30, ('segunda', 'terça', 'quarta','quinta')))

menor, maior = get_maior_menor(tss)
print(f"ID da sessão com menor custo: {menor[0]} | Custo:{menor[1]}\nID da sessão com maior custo: {maior[0]} | Custo: {maior[1]}")

maior = get_mais_dias(tss)
print(f"ID da sessão com mais dias: {maior[0]} | Dias: {maior[1]} {get_dias(get_session(maior[0], tss))}")

print("Sistema Planetário Rio\n")
id = int(input("Digite o seu identificador: "))
total = 0
while id != 0:
    sessao = int(input("Digite o identificador da sessão: "))
    s = get_session(sessao, tss)
    nome = get_name(s)
    custo = get_custo(s)
    horaminuto = get_horaminuto(s)
    hora = horaminuto[0]
    minuto = horaminuto[1]
    dias = get_dias(s)
    print(f"Nome da sessão: {nome} | Duração: {hora}:{minuto}")
    print("Dias disponíveis:")
    for dia in dias:
        print(f"- {dia}")
    print(f"Custo: R${custo}")
    total += custo
    id = int(input("Digite o seu identificador: "))
print(f"Total: R${total}")