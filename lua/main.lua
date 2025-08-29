-- Exerc 1
tabela = {
  "P1",
  "P2",
  "P3",
  "P4",
  "P5",
  "RJS-01",
  "P10",
  "P67",
  "P101",
}

print("Exerc 1\n")
table.sort(tabela, function(a, b) -- Sort recebe função como parâmetro
  local a_str = string.match(a, "%d+") -- Um ou mais dígitos
  local b_str = string.match(b, "%d+") -- Um ou mais dígitos
  if not a_str or not b_str then return a < b end -- Se forem nill retorna o default
  local a_num = tonumber(a_str) -- Conversão para número
  local b_num = tonumber(b_str) -- Conversão para número
  return a_num < b_num end) -- Verifica qual dos 2 números é menor

for i = 1, #tabela do
  print(tabela[i]) -- Exibe a table
end

-- Exerc 2
print("\nExerc 2")

file = io.open("TESTE_LUA.txt", "r") if not file then print("Erro ao abrir o arquivo") end -- Abertura do arquivo com tratamento de erro

table = {}
cabecalho = file:read("*l") -- Lê a primeira linha do arquivo
unidades = file:read("*l") -- Lê a segunda linha do arquivo
column_list = {}

for column in string.gmatch(cabecalho, "%S+") do
  table[column] = {} -- Colunas dentro da table sendo criadas
  column_list[#column_list+1] = column
  if column ~= "DATA" then -- DATA não possui unidade
    unidade = string.match(unidades, "%S+")
    table[column].unidade = unidade -- Campo unidade dentro de cada coluna
  end
end

for linha in file:lines() do
  data = string.match(linha, "(%d%d%d%d/%d%d/%d%d)") -- Datas tem um formato único
  table["DATA"][#table["DATA"]+1] = data
  for i = 2, #column_list do
    value = string.match(linha, "%d+%.%d") -- Outros valores são racionais com uma casa decimal
    table[column_list[i]][#table[column_list[i]]+1] = value -- Valores são armazenados na table como lista
  end
end

file:close()

-- Exibir table
for n = 1, #column_list do
  column = column_list[n]
  print("\nColuna: "..column) -- Exibe a chave da coluna
  if column ~= "DATA" then -- DATA não possui unidade
     print("Unidade: "..table[column_list[n]].unidade.."\n") -- Exibe unidade do restante das colunas
  end
  for i = 1, #table[column] do 
    io.write(table[column][i].." | ") -- Exibe todas as linhas com io.write para evitar o \n final do print
  end
  print("\n")
end