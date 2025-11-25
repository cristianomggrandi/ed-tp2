import subprocess
import matplotlib.pyplot as plt
import numpy as np

# --- CONFIGURAÇÕES BASE ---
EXECUTAVEL = "./tp2.exe"
ARQUIVO_DEMANDAS = "entradas-saidas/input_1000.txt"

# Parâmetros fixos base (conforme seu exemplo)
BASE_ETA = 3       # Capacidade
BASE_GAMA = 35.0   # Velocidade
BASE_DELTA = 30.0  # Tempo max espera
BASE_ALFA = 1500.0 # Max dist origem
BASE_BETA = 3000.0 # Max dist destino
BASE_LAMBDA = 0.1  # Eficiência mínima

# Lê as demandas do arquivo para memória
with open(ARQUIVO_DEMANDAS, 'r') as f:
    DEMANDAS_RAW = f.read()
    NUM_DEMANDAS = len(DEMANDAS_RAW.strip().split('\n'))

def rodar_simulacao(eta, gama, delta, alfa, beta, lamb, num_demandas):
    """
    Monta a string de entrada, roda o programa C e retorna métricas.
    """
    # Monta a primeira linha de input (parâmetros)
    input_str = f"{eta}\n{gama}\n{delta}\n{alfa}\n{beta}\n{lamb}\n{num_demandas}\n"
    # Adiciona as demandas
    input_str += DEMANDAS_RAW
    
    # Executa o processo
    processo = subprocess.Popen(
        [EXECUTAVEL],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = processo.communicate(input=input_str)
    
    # --- PARSE DO OUTPUT ---
    # Baseado no seu minheap.h/finalize: 
    # printf("%.2f %.2f %d ...", ride.end_time, total_distance, ride.stop_number);
    
    linhas = stdout.strip().split('\n')
    linhas = [l for l in linhas if l.strip()] # Remove linhas vazias
    
    num_corridas = len(linhas)
    distancia_total_frota = 0.0
    corridas_agrupadas = 0
    corridas_individuais = 0
    
    for l in linhas:
        try:
            partes = l.split()
            # O segundo elemento é a distância total daquela corrida
            distancia_total_frota += float(partes[1])
            # O terceiro elemento é o número de paradas (stop_number)
            num_paradas = int(partes[2])
            # Se tem mais de 1 parada, é uma corrida agrupada
            if num_paradas > 1:
                corridas_agrupadas += 1
            else:
                corridas_individuais += 1
        except:
            pass # Ignora linhas de debug se houver
            
    return num_corridas, distancia_total_frota, corridas_agrupadas, corridas_individuais

# --- EXPERIMENTO 1: Variação Espacial (Alpha e Beta) ---
# Relaxando restrições de distância para ver se aumenta o compartilhamento
print("Rodando Experimento 1: Variação de Alpha/Beta...")
alfas = np.linspace(0, 500, 20) # Testa 15 valores de 0 a 500
resultados_corridas_e1 = []
resultados_distancia_e1 = []
resultados_agrupadas_e1 = []
resultados_individuais_e1 = []

for val in alfas:
    # Vamos variar Alpha e Beta juntos proporcionalmente (Beta = 2 * Alpha por exemplo)
    # ou manter Beta fixo. Vamos variar Alpha mantendo Beta fixo conforme sugestão 3.6.
    n_corridas, dist_total, agrupadas, individuais = rodar_simulacao(
        BASE_ETA, BASE_GAMA, BASE_DELTA, 
        val, BASE_BETA, BASE_LAMBDA, NUM_DEMANDAS
    )
    resultados_corridas_e1.append(n_corridas)
    resultados_distancia_e1.append(dist_total)
    resultados_agrupadas_e1.append(agrupadas)
    resultados_individuais_e1.append(individuais)

# --- EXPERIMENTO 2: Variação de Beta (Distância Máxima Destino) ---
print("Rodando Experimento 2: Variação de Beta...")
betas = np.linspace(0, 500, 20) # Testa 15 valores de 100 a 5000
resultados_corridas_e2 = []
resultados_distancia_e2 = []
resultados_agrupadas_e2 = []
resultados_individuais_e2 = []

for val in betas:
    n_corridas, dist_total, agrupadas, individuais = rodar_simulacao(
        BASE_ETA, BASE_GAMA, BASE_DELTA, 
        BASE_ALFA, val, BASE_LAMBDA, NUM_DEMANDAS
    )
    resultados_corridas_e2.append(n_corridas)
    resultados_distancia_e2.append(dist_total)
    resultados_agrupadas_e2.append(agrupadas)
    resultados_individuais_e2.append(individuais)

# --- EXPERIMENTO 3: Variação de Eficiência (Lambda) ---
print("Rodando Experimento 3: Variação de Lambda...")
lambdas = np.linspace(0.1, 1.5, 15) # Eficiência de 0.1 até 1.5
resultados_corridas_e3 = []
resultados_distancia_e3 = []
resultados_agrupadas_e3 = []
resultados_individuais_e3 = []

for val in lambdas:
    n_corridas, dist_total, agrupadas, individuais = rodar_simulacao(
        BASE_ETA, BASE_GAMA, BASE_DELTA, 
        BASE_ALFA, BASE_BETA, val, NUM_DEMANDAS
    )
    resultados_corridas_e3.append(n_corridas)
    resultados_distancia_e3.append(dist_total)
    resultados_agrupadas_e3.append(agrupadas)
    resultados_individuais_e3.append(individuais)

# --- PLOTAGEM DOS GRÁFICOS ---
# Grafico 1: Alpha vs Num Corridas
plt.figure(figsize=(8, 6))
plt.plot(alfas, resultados_corridas_e1, marker='o', color='b')
plt.title('Impacto da Dist. Origem (Alpha) no Compartilhamento')
plt.xlabel('Alpha (metros)')
plt.ylabel('Número de Corridas (Menos é melhor)')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_alpha_corridas.png')
plt.close()

# Grafico 2: Alpha vs Distancia Total
plt.figure(figsize=(8, 6))
plt.plot(alfas, resultados_distancia_e1, marker='s', color='r')
plt.title('Impacto da Dist. Origem (Alpha) na Distância da Frota')
plt.xlabel('Alpha (metros)')
plt.ylabel('Distância Total Percorrida')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_alpha_distancia.png')
plt.close()

# Grafico 3: Beta vs Num Corridas
plt.figure(figsize=(8, 6))
plt.plot(betas, resultados_corridas_e2, marker='o', color='purple')
plt.title('Impacto da Dist. Destino (Beta) no Compartilhamento')
plt.xlabel('Beta (metros)')
plt.ylabel('Número de Corridas')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_beta_corridas.png')
plt.close()

# Grafico 4: Beta vs Distancia Total
plt.figure(figsize=(8, 6))
plt.plot(betas, resultados_distancia_e2, marker='s', color='cyan')
plt.title('Impacto da Dist. Destino (Beta) na Distância da Frota')
plt.xlabel('Beta (metros)')
plt.ylabel('Distância Total Percorrida')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_beta_distancia.png')
plt.close()

# Grafico 5: Lambda vs Num Corridas
plt.figure(figsize=(8, 6))
plt.plot(lambdas, resultados_corridas_e3, marker='o', color='g')
plt.title('Impacto da Eficiência Mínima (Lambda)')
plt.xlabel('Lambda')
plt.ylabel('Número de Corridas')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_lambda_corridas.png')
plt.close()

# Grafico 6: Lambda vs Distancia Total
plt.figure(figsize=(8, 6))
plt.plot(lambdas, resultados_distancia_e3, marker='s', color='orange')
plt.title('Impacto da Eficiência Mínima na Distância da Frota')
plt.xlabel('Lambda')
plt.ylabel('Distância Total Percorrida')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_lambda_distancia.png')
plt.close()

# --- GRÁFICOS DE PROPORÇÃO: Corridas / Demandas ---
# Calcular proporção de corridas sobre demandas (quanto menor, mais agrupamento)

# Proporções para Alpha
proporcoes_e1 = [(corridas / NUM_DEMANDAS) for corridas in resultados_corridas_e1]

# Grafico 7: Alpha vs Proporção Corridas/Demandas
plt.figure(figsize=(8, 6))
plt.plot(alfas, proporcoes_e1, marker='o', color='#2ecc71', linewidth=2)
plt.title('Proporção Corridas/Demandas vs Alpha')
plt.xlabel('Alpha (metros)')
plt.ylabel('Proporção (Corridas / Demandas)')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_alpha_proporcao.png')
plt.close()

# Proporções para Beta
proporcoes_e2 = [(corridas / NUM_DEMANDAS) for corridas in resultados_corridas_e2]

# Grafico 8: Beta vs Proporção Corridas/Demandas
plt.figure(figsize=(8, 6))
plt.plot(betas, proporcoes_e2, marker='o', color='#9b59b6', linewidth=2)
plt.title('Proporção Corridas/Demandas vs Beta')
plt.xlabel('Beta (metros)')
plt.ylabel('Proporção (Corridas / Demandas)')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_beta_proporcao.png')
plt.close()

# Proporções para Lambda
proporcoes_e3 = [(corridas / NUM_DEMANDAS) for corridas in resultados_corridas_e3]

# Grafico 9: Lambda vs Proporção Corridas/Demandas
plt.figure(figsize=(8, 6))
plt.plot(lambdas, proporcoes_e3, marker='o', color='#e67e22', linewidth=2)
plt.title('Proporção Corridas/Demandas vs Lambda')
plt.xlabel('Lambda')
plt.ylabel('Proporção (Corridas / Demandas)')
plt.grid(True)
plt.tight_layout()
plt.savefig('grafico_lambda_proporcao.png')
plt.close()

print("Análise concluída. Gráficos salvos:")
print("  - grafico_alpha_corridas.png")
print("  - grafico_alpha_distancia.png")
print("  - grafico_alpha_proporcao.png")
print("  - grafico_beta_corridas.png")
print("  - grafico_beta_distancia.png")
print("  - grafico_beta_proporcao.png")
print("  - grafico_lambda_corridas.png")
print("  - grafico_lambda_distancia.png")
print("  - grafico_lambda_proporcao.png")