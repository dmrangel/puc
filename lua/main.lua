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

print("\nExerc 2\n")






