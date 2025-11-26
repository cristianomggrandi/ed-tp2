#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#ifndef TYPES_H
#include "types.h"
#endif

// Valida os parâmetros de configuração do sistema (eta > 0, gamma > 0, delta >= 0, alpha >= 0, beta >= 0, lambda >= 0)
bool validate_config_params(int eta, double gamma, double delta, double alpha, double beta, double lambda)
{
    if (eta <= 0)
    {
        printf("\nERRO: Capacidade (eta) deve ser positiva. Valor recebido: %d", eta);
        return false;
    }

    if (eta > MAX_CAPACITY)
    {
        printf("\nERRO: Capacidade (eta) excede limite máximo (%d). Valor recebido: %d", MAX_CAPACITY, eta);
        return false;
    }

    if (gamma <= 0)
    {
        printf("\nERRO: Velocidade (gamma) deve ser positiva. Valor recebido: %.2f", gamma);
        return false;
    }

    if (delta < 0)
    {
        printf("\nERRO: Tempo máximo de espera (delta) não pode ser negativo. Valor recebido: %.2f", delta);
        return false;
    }

    if (alpha < 0)
    {
        printf("\nERRO: Distância máxima de origem (alpha) não pode ser negativa. Valor recebido: %.2f", alpha);
        return false;
    }

    if (beta < 0)
    {
        printf("\nERRO: Distância máxima de destino (beta) não pode ser negativa. Valor recebido: %.2f", beta);
        return false;
    }

    if (lambda < 0)
    {
        printf("\nERRO: Eficiência mínima (lambda) deve ser maior ou igual a 0. Valor recebido: %.2f", lambda);
        return false;
    }

    return true;
}

// Valida uma demanda individual (coordenadas válidas, origem != destino, tempo >= 0)
bool validate_demand(Demand demand)
{
    // Verifica se as coordenadas são números válidos
    if (isnan(demand.origin.x) || isnan(demand.origin.y) || 
        isinf(demand.origin.x) || isinf(demand.origin.y))
    {
        printf("\nERRO: Coordenadas de origem inválidas para demanda %d: (%.2f, %.2f)", 
               demand.id, demand.origin.x, demand.origin.y);
        return false;
    }

    if (isnan(demand.destination.x) || isnan(demand.destination.y) || 
        isinf(demand.destination.x) || isinf(demand.destination.y))
    {
        printf("\nERRO: Coordenadas de destino inválidas para demanda %d: (%.2f, %.2f)", 
               demand.id, demand.destination.x, demand.destination.y);
        return false;
    }

    // Verifica se origem e destino são diferentes
    double distance = hypot(demand.origin.x - demand.destination.x, 
                           demand.origin.y - demand.destination.y);
    if (distance < 0.0001) // Tolerância para comparação de floats
    {
        printf("\nERRO: Origem e destino são idênticos para demanda %d", demand.id);
        return false;
    }

    // Verifica se o tempo é não-negativo
    if (demand.time < 0)
    {
        printf("\nERRO: Tempo de solicitação negativo para demanda %d: %d", demand.id, demand.time);
        return false;
    }

    return true;
}

// Valida o número total de demandas (deve ser > 0, avisa se > 100000)
bool validate_demand_count(int num_demands)
{
    if (num_demands <= 0)
    {
        printf("\nERRO: Número de demandas deve ser positivo. Valor recebido: %d", num_demands);
        return false;
    }

    if (num_demands > 100000) // Limite razoável para evitar problemas de memória
    {
        printf("\nAVISO: Número de demandas muito alto (%d). Isso pode causar problemas de desempenho.", num_demands);
    }

    return true;
}

#endif
