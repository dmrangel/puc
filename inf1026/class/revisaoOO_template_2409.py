# -*- coding: utf-8 -*-
"""
Created on Sun Sep 22 11:50:00 2024

@author: Ferlin
"""
#Considere a classe Oficina fornecida no arquivo classeOficina.py
 # Esta classe representa uma oficina que realiza serviços e cobra por hora. 
 # Ela contém os seguintes atributos:
 #       nome: nome da oficina (uma string).
 #       valor_minuto: valor cobrado pela oficina por cada minuto de serviço (um número). 
 # Métodos:
 #     construtor: recebe o nome da oficina e o valor por hora(default: 100)
 #                 cria e retorna um obj
 #     exibição (__str__): retorna uma string de exibição da oficina no formato:
 #                 "Oficina <nome> - Valor da hora: R$<valor_hora>".   
 #              (__repr__):retorna uma representação da oficina no formato:
 #                          "Oficina <nome> - R$<valor_hora>/hora".
 #      getNome: Retorna o nome da oficina.
 #      getValorMinuto Retorna o valor cobrado por minuto de serviço da oficina.
 #      setValorMinuto: recebe um valor e altera o valor cobrado por minuto de serviço da oficina, 
      
#Considere a classe Horario fornecida no arquivo classeHorario.py
 # Esta classe representa uma unidade de tempo,representando um horário 
 # composto por horas e minutos. Inicializa o objeto com o horário atual do 
 # sistema quando os parâmetros não são fornecidos.
 # Ela contém os seguintes atributos:
 #       horas: Representa a parte das horas do horário, um número inteiro no intervalo de 0 a 23, seguindo o formato de 24 horas.
 #       minutos: Representa a parte dos minutos, um número inteiro no intervalo de 0 a 59.
 # Métodos:
 #     construtor: cria um obj Horario com as horas e minutos fornecidos 
 #                 como parâmetros.Pode ser instanciado apenas com a hora
 #                 ou apenas com minutos.Se instanciado sem parâmetros assume o 
 #                 horário da criação do objeto (atual do sistema), (valor default)
 #                 
 #                 
 #     exibição (__str__): retorna uma string de exibição do horario no formato:
 #                "hh:mm"
 #              
 #      + : recebe uma quantidade de minutos e retorna um novo objeto com horas e minutos atuaizados
 #      < : compara dois objetos Horario. Retorna True se o horário atual for menor 
 #          que o outro horário, o que significa que ocorre antes. (minutos totais menor)
 #      totalMinutos: retorna o total de minutos do horário atual, somando as 
 #          horas convertidas em minutos e os minutos do horário.
     

from classeOficina import *
from classeHorario import *

# DESENVOLVA AS SEGUINTES CLASSES: 
    
# Classe Servico 
# A classe Servico serve para representar serviços de manutenção oferecidos por uma empresa
# Um serviço tem um nome e um tempo (objeto da classe Horario) de mão de obra necessário para realizá-lo.

# Um serviço é criado fornecendo seu nome e o tempo necessário.(default 60 minutos)
# => método __init__

# Ao se dar print em um serviço, deve-se exibir o nome e o valor no seguinte formato:
#               xxx - tempo necessário: hh:mm
# onde xxx é o nome do serviço e
#      hh:mm é o tempo necessário    
# exemplo: "Troca de Filtro" levando 65 minutos --> Troca de Filtro - tempo necessário: 01:05h
# => método __str__

# O método __repr__ é idêntico ao __str__:

# Métodos de consulta e alteração devem ser fornecidos para o nome e tempo.
# => Métodos getNomeDoServico, getDuracaoDoServico, 
# => setDuracaoDoServico, recebe um obj Horario e atualiza a duração do serviço
# => calcularValorServico: recebe o valor por minuto e retorna o valor total do serviço






# Classe ServicoEspecial
# A classe ServicoEspecial é um tipo de Servico, portanto, tem tudo que a 
# classe serviço tem  mais uma taxa extra 
# Um serviço especial é criado fornecendo seu nome e o tempo necessário.(default 60 minutos)
# e uma taxa extra (em reais) com valor default = 50
# => método __init__

# Ao se dar print em um serviço, deve-se exibir o nome e o valor no seguinte formato:
#               xxx - tempo necessário: hh:mm taxa extra: R$ yyy.yy
# onde xxx é o nome do serviço e
#      hh:mm é o tempo necessário  
#      yyy.yy é a taxa extra  
# exemplo: "Troca de Filtro" levando 65 minutos --> Troca de Filtro - tempo necessário: 01:05h
# => método __str__

# O método __repr__ é similar ao __str__ mas a taxa extra está em uma nova linha:

# Métodos de consulta e alteração devem ser fornecidos para o nome, tempo e taxa extra.
# => Métodos getNomeDoServico, getDuracaoDoServico,getTaxaExtra 
# => setDuracaoDoServico, recebe um obj Horario e atualiza a duração do serviço
# => setTaxaExtra, recebe um valor e  atualiza a taxa extra para um serviço especial.
# => calcularValorTotal: recebe o valor por minuto e retorna o valor total do servico com a taxa extra







# Classe PedidoManutencao

# A classe PedidoManutencao representa um pedido de manutenção feito por um cliente
# o valor da hora de mao de obra depende da oficina que o realiza
# Um pedido tem:
# - código (inteiro): Um número inteiro que identifica de forma única o pedido de manutenção.
# - nome do cliente (str): O nome do cliente que solicitou o pedido de manutenção.
# - oficina (objeto Oficina):  a oficina que executará o serviço. 
# - lista de serviços (lista de objetos Servico), com todos os serviços que 
#       fazem parte do pedido de manutenção,
# - horarioInicio (objeto Horario):
# - horarioPrevistoConclusao (objeto Horario): horário estimado para a conclusão 
#               do pedido de manutenção, baseado na soma do tempo necessário 
#               para executar todos os serviços incluídos.
# - horarioConclusao (objeto Horario): o horário em que o pedido foi efetivamente concluído.
# - situacao (inicialmente sempre igual a 'iniciado'): ndica o estado atual do
#          pedido. Os valores possíveis são "Iniciado" ou "Terminado" 

# Um pedido é criado fornecendo o código, nome do cliente, a oficina, 
#                                o horário de início e a lista de serviços. 
#       A situação inicial é "Iniciado",  
#       O horário previsto de conclusão é calculado com base na duração dos serviços.
#       Caso a lista de serviços esteja vazia, o tempo previsto é de 180 minutos
#       O horário de conclusão é inciado com None
# => método __init__

# Na exibição de um pedido é mostrado o código, o nome do cliente, o nome da oficina, e o horário previsto para a conclusão.
# => métodos __str__   e __repr__

# Demais Métodos:
    
# =>incluiServico: recebe um obj Servico e o adiciona à lista de serviços do pedido. 
#             Esse método também atualiza o horárioPrevistoConclusao para refletir o 
#             tempo adicional necessário para concluir o serviço recém-adicionado. 
  
# =>exibeServicos: exibe todos os serviços associados ao pedido de manutenção

# =>tempoTotalDoPedido: calcula o tempo total de execução dos serviços incluídos 
#           no pedido, somando a duração de cada serviço. 
#            O resultado é um objeto Horario que representa o tempo total em horas e minutos.

# => exibeValorTotalDoPedido: calcula e exibe o valor total do pedido de manutenção. 
#            O cálculo é feito multiplicando a duração total dos serviços pelo valor
#            cobrado por minuto da oficina, somando taxas extras no caso de serviços especiais.
#
# => concluirPedido: receb um obj Horario. Marca o pedido como concluído, definindo
#           o horarioConclusao como o horário recebido (em que a manutenção foi finalizada)
#           e atualizando o status do pedido para "Terminado".

# => exibeSituacaoDoPedido: exibe a situação atual do pedido de manutenção.
#           Mostra o codigo, o cliente e o horário previsto do pedido e:
#          para pedidos concluídos: o horário de conclusão e se foi concluído 
#                                   dentro do prazo, antes do prazo, ou atrasado 
#                                   em relação ao horário previsto de conclusão.
#           para pedidos ainda não concluídos:o horario previsto de conclusão e 
#                                   a mesnagem PEDIDO EM ANDAMENTO
#            """




from classeOficina import Oficina
from classeHorario import Horario


# -*- coding: utf-8 -*-
"""
Created on Sun Sep 22 11:50:00 2024

@author: Ferlin
"""

# Tire o comentarios para testar as funções de suas classes
'''
# Criando uma oficina
oficina = Oficina("Oficina ABC", valor_min=1.67)  # Valor por minuto (R$ 1.67/min = R$ 100/hora)

# Criando o horário de início do pedido de manutenção
horario_inicio = Horario(9, 0)  # Horário de início às 9h da manhã

# Criando serviços normais
servico1 = Servico("Troca de Óleo", Horario(minutos=45))  # Serviço de 45 minutos
servico2 = Servico("Alinhamento", Horario(horas=1, minutos=30))  # Serviço de 1 hora e 30 minutos

# Criando um serviço especial com taxa extra
servico_especial = ServicoEspecial("Troca de Suspensão", Horario(horas=2), taxa_extra=150.00)  # Serviço de 2 horas com taxa extra de R$50

# Criando o pedido de manutenção
pedido = PedidoManutencao(101, "Carlos Silva", oficina, horario_inicio, [servico1, servico2])

# Exibindo informações do pedido
print("Informações do Pedido:")
print(pedido)

# Exibindo os serviços no pedido
print("\nServiços associados ao pedido:")
pedido.exibeServicos()

# Exibindo o valor total do pedido antes de adicionar o serviço especial
print("\nValor total do pedido antes de adicionar serviço especial:")
pedido.exibeValorTotalDoPedido()

# Adicionando o serviço especial ao pedido
pedido.incluiServico(servico_especial)

# Exibindo os serviços após a adição de um novo serviço especial
print("\nServiços após adição de serviço especial:")
pedido.exibeServicos()

# Exibindo o valor total do pedido com o serviço especial
print("\nNovo valor total do pedido após adicionar o serviço especial:")
pedido.exibeValorTotalDoPedido()

# Concluindo o pedido de manutenção
horario_conclusao = Horario(12, 0)  # O pedido foi concluído às 12h
pedido.concluirPedido(horario_conclusao)

# Verificando a situação do pedido (se foi concluído no prazo, antes ou atrasado)
print("\nSituação do pedido:")
pedido.exibeSituacaoDoPedido()
'''