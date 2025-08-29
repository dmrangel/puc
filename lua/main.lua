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
table.sort(tabela, function(a, b)
  local a_str = string.match(a, "%d+")
  local b_str = string.match(b, "%d+")
  if not a_str or not b_str then return a < b end
  local a_num = tonumber(a_str)
  local b_num = tonumber(b_str)
  return a_num < b_num end)

for i = 1, #tabela do
  print(tabela[i])
end

-- Exerc 2
print("\nExerc 2\n")

file = io.open("TESTE_LUA.txt", "r") if not file then print("Erro ao abrir o arquivo") end

table = {}
cabecalho = file:read("*l")
unidades = file:read("*l")
column_list = {}
column = string.match(cabecalho, "%S+")

for column in string.gmatch(cabecalho, "%S+") do
  table[column] = {}
  column_list[#column_list+1] = column
  if column ~= "DATA" then
    unidade = string.match(unidades, "%S+")
    table[column] = unidade
  end
end
for linha in file:lines() do
  data = string.match(linha, "(%d%d%d%d/%d%d/%d%d)")
  print(data)
  table["DATA"][#table["DATA"]+1] = data
  for i = 2, #column_list do
    value = string.match(linha, "%d+%.%d")
    print(value)
    table[column_list[i]][#table[column_list[i]]+1] = value
  end
end

file:close()