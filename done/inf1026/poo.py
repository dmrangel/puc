class Ponto():
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return (f"Ponto: {self.x, self.y}")
    
    def exibe(self):
        print(f"Ponto: {self.x, self.y}")

    def distancia(self, p2):
        distancia = ((p2.x - self.x)**2 + (p2.y - self.y)**2)**0.5
        print(f"Distancia entre {self.x, self.y} e {p2.x, p2.y}: {distancia}")

    def distanciaOrigem(self):
        distancia = (self.x**2 + self.y**2)**0.5
        print(f"Distancia de {self.x, self.y} da Origem: {distancia}")

p1 = Ponto(10,5)
p2 = Ponto(-10, -5)
p1.exibe()
print(p1)
p1.distancia(p2)
p1.distanciaOrigem()