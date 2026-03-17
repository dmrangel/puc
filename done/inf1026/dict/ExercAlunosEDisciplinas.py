def criaDicDisciplinasDoAluno(end):
    arq = open(end,'r')
    dicDiscDoAluno = dict()
    for linha in arq:
        lAluno = linha.strip().split(';')
        (mAluno, lDiscAluno) = (int(lAluno[0]), lAluno[1:])
        dicDiscDoAluno[mAluno] = lDiscAluno
    arq.close()
    return dicDiscDoAluno

dDiscDoAluno = criaDicDisciplinasDoAluno('inscritos.csv')
print('\n   ALUNO: disciplinas em que está inscrito')
for aluno in dDiscDoAluno:
    print(aluno,":",dDiscDoAluno[aluno])

# =================================

def criaDicAlDaDisc(dDiscDoAl):
    dAlunosDaDisc = dict()
    for aluno in dDiscDoAl:
        for disc in dDiscDoAl[aluno]:
            lstAlunosDaDisc = dAlunosDaDisc.get(disc, [])
            lstAlunosDaDisc.append(aluno)
            dAlunosDaDisc[disc] = lstAlunosDaDisc
    return dAlunosDaDisc


def criaDicAlDaDisc2(dDiscDoAl):
    dAlunosDaDisc = dict()
    for k,v in dDiscDoAl.items():
        for e in v:
            if e in dAlunosDaDisc:
                dAlunosDaDisc[e].append(k)
            else:
                dAlunosDaDisc[e] = [k]
    return dAlunosDaDisc


dAlunosDaDisc = criaDicAlDaDisc(dDiscDoAluno)
# dAlunosDaDisc = criaDicAlDaDisc2(dDiscDoAluno)
print('\n   DISCIPLINA: alunos inscritos')
for disc in dAlunosDaDisc:
    print(disc, ":", dAlunosDaDisc[disc])

# ==========================================

def atualizaDicAlunosDaDisc(dAlunosDaDisc, lista_matr):
    for v in dAlunosDaDisc.values():
       v_copy = v.copy()
       # for e in v: #não percorre todos os elementos pois estou eliminando enquanto estou iterando na mesma coleçao, necesita da copia
       for e in v_copy:
           if e in lista_matr:
               v.remove(e)


lista_matricula = [1612299, 1413399, 1619999]
print('\n   Lista de matriculas a serem eliminadas\n', lista_matricula)
atualizaDicAlunosDaDisc(dAlunosDaDisc, lista_matricula)
print('\n   DISCIPLINA: alunos inscritos depois de remover a lista')
for disc in dAlunosDaDisc:
    print(disc, ":", dAlunosDaDisc[disc])

# =====================================

def atualizaDicDisciplinasDoAluno(dicDiscDoAluno, lista_matr):
    dicDiscDoAluno_copy = dicDiscDoAluno.copy()
    for k in dicDiscDoAluno_copy:
        if k in lista_matr:
            del dicDiscDoAluno[k]

#outra versao
def atualizaDicDisciplinasDoAluno2(dicDiscDoAluno, lista_matr):
    for k in lista_matr:
        if k in dicDiscDoAluno:
            del dicDiscDoAluno[k]

lista_matricula = [1612299, 1413399, 1619999]
print('\n   Lista de matriculas a serem eliminadas\n', lista_matricula)
atualizaDicDisciplinasDoAluno2(dDiscDoAluno, lista_matricula)

print('\n   ALUNO: disciplinas em que está inscrito')
for aluno in dDiscDoAluno:
    print(aluno,":",dDiscDoAluno[aluno])