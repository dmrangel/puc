def criaItemAluno(dadosUmAluno):
  matAluno = int(dadosUmAluno[0])
  dDesemp = {};
  tam = len(dadosUmAluno)
  # print(range(1,tam,2))
  for i in range(1,tam,2):
    dDesemp[dadosUmAluno[i]]=float(dadosUmAluno[i+1])
  return (matAluno, dDesemp)

def criaDicDisciplinasDoAluno(nomeArq):
    arq = open(nomeArq,'r')
    dHistoricos = {}
    for linha in arq:
      (mAluno, dDesempAluno)= criaItemAluno((linha.strip()).split(';'))
      dHistoricos[mAluno]= dDesempAluno
    arq.close()
    return dHistoricos


dicHistorico = criaDicDisciplinasDoAluno('HistoricoNotas.csv')
print("ALUNO e seu historico")
for aluno in dicHistorico:
    print(aluno,":",dicHistorico[aluno])



# ===============

def criaDicAlNtDaDisc(dHistorico):
    dDisc = {}
    for al in dHistorico:
        for disc in dHistorico[al]:
            if disc not in dDisc:
                dDisc[disc] = {al: dHistorico[al][disc]}
            else:
                dDisc[disc].update({al: dHistorico[al][disc]})
    return dDisc


def criaDicAlNtDaDisc2(dHistorico):
    dDisc = {}
    for k,v in dHistorico.items():
        for k1,v1 in v.items():
            if k1 not in dDisc:
                dDisc[k1] = {k:v1}
            else:
                dDisc[k1][k] = v1
    return dDisc



dAlunoseNotasDaDisc = criaDicAlNtDaDisc(dicHistorico)
# dAlunoseNotasDaDisc = criaDicAlNtDaDisc2(dicHistorico)
print('\n DISCIPLINA e seus alunos')
for disc in dAlunoseNotasDaDisc:
    print(disc, ":", dAlunoseNotasDaDisc[disc])
