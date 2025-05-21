# SOchain - Simulador de Transações com Token SOT

## Introdução
O SOchain é uma aplicação em C que simula um sistema de transações para uma criptomoeda chamada **SOT (SO Token)**. O sistema incorpora conceitos de sistemas operativos como gestão de processos, memória partilhada, sincronização de processos (com semáforos), sinais, alarmes e operações de ficheiros.

Este projeto foi desenvolvido no âmbito da unidade curricular de Sistemas Operativos da Faculdade de Ciências da Universidade de Lisboa.

## Estrutura do Projeto
O projeto segue a seguinte estrutura de diretórios:
```
SOCHAIN/
|-- bin/                # Ficheiros executáveis
|-- inc/                # Ficheiros de cabeçalho (.h)
|-- obj/                # Ficheiros objeto (.o)
|-- src/                # Ficheiros de código-fonte (.c)
|-- makefile            # Script de compilação
|-- README.md           # Este documento
|-- args.txt            # Ficheiro de argumentos de entrada
|-- settings.txt        # Ficheiro de definições (log, stats, período)
```

## Compilação
Para gerir o projeto, utiliza os seguintes comandos:
- **Compilar o projeto:** `make all`
- **Compilar apenas:** `make`
- **Limpar ficheiros compilados:** `make clean`
- **Recompilar do zero:** `make re`

## Execução

**Fase 2:**
Executa o programa com:
```bash
./bin/SOchain args.txt settings.txt
```
Onde:
- `args.txt` contém, linha a linha:
    1. Saldo inicial para cada carteira (`init_balance`)
    2. Número de carteiras (`n_wallets`)
    3. Número de servidores (`n_servers`)
    4. Tamanho do buffer (`buff_size`)
    5. Número máximo de transações (`max_txs`)
- `settings.txt` contém, linha a linha:
    1. Nome do ficheiro de log (ex: `log.txt`)
    2. Nome do ficheiro de estatísticas (ex: `stats.txt`)
    3. Período para impressão periódica de estatísticas (em segundos, usa `0` para desativar)

**Exemplo de `args.txt`:**
```
100.0
2
2
10
50
```
**Exemplo de `settings.txt`:**
```
log.txt
stats.txt
5
```

### Exemplo de Execução
```bash
./bin/SOchain args.txt settings.txt
```

## Funcionalidades

O programa disponibiliza um menu interativo para gerir transações:
- `bal <id>`: Verifica o saldo de uma carteira específica.
- `trx <src_id> <dest_id> <amount>`: Cria uma transação de uma carteira para outra.
- `rcp <id>`: Obtém o recibo de uma transação.
- `stat`: Mostra o estado atual do sistema.
- `help`: Mostra a lista de comandos disponíveis.
- `end`: Termina a execução do SOchain.

### Novidades da Fase 2
- **Sincronização:** Todo o acesso à memória partilhada é protegido por semáforos (modelo produtor/consumidor).
- **Leitura de Ficheiros:** Os argumentos são lidos de `args.txt` e as definições de `settings.txt`.
- **Registo de Operações:** Todas as operações do utilizador são registadas no ficheiro especificado em `settings.txt` com marca temporal.
- **Estatísticas Periódicas:** Se o período em `settings.txt` for maior que 0, o sistema imprime periodicamente o estado das transações.
- **Gestão de Sinais:** O programa trata o sinal `SIGINT` (CTRL+C) para terminar de forma limpa, libertando todos os recursos.
- **Ficheiro de Estatísticas:** No final, as estatísticas são escritas no ficheiro especificado em `settings.txt`.

## Módulos Principais
- `main.c/h`: Gestão da criação de processos e interação com o utilizador.
- `process.c/h`: Gestão da criação e terminação de processos.
- `memory.c/h`: Gestão de memória dinâmica e partilhada.
- `wallet.c/h`: Assinatura de transações e envio para os servidores.
- `server.c/h`: Validação, processamento e registo de transações.
- `synchronization.c/h`: Implementação de semáforos para sincronização.
- `clog.c/h`: Registo das operações do utilizador.
- `csettings.c/h`: Leitura dos ficheiros de configuração.
- `ctime.c/h`: Gestão de timestamps das transações.
- `csignal.c/h`: Gestão de sinais e alarmes.
- `cstats.c/h`: Escrita das estatísticas finais.

## Testes

Para testar o programa:
1. **Compilar:**
   ```bash
   make
   ```
2. **Preparar ficheiros de entrada:**
   Edita `args.txt` e `settings.txt` conforme os exemplos acima.
3. **Executar:**
   ```bash
   ./bin/SOchain args.txt settings.txt
   ```
4. **Interagir:**
   Usa os comandos (`bal`, `trx`, `rcp`, `stat`, `help`, `end`) conforme descrito acima.
5. **Verificar saídas:**
   - Ficheiro de log (ex: `log.txt`) para histórico de operações.
   - Ficheiro de estatísticas (ex: `stats.txt`) para estatísticas finais.
   - Estatísticas periódicas no terminal se ativado.
6. **Testar sinais:**
   Pressiona `CTRL+C` para garantir que o programa termina corretamente e escreve as estatísticas.

## Autores
**Grupo 40**
- Rodrigo Frutuoso - 61865
- Simão Alexandre - 61874
- Tiago Leite - 61863

## Notas
- O projeto foi desenvolvido e testado em ambiente Linux.
- Toda a sincronização, logging e gestão de sinais está implementada nesta fase.

## Licença
Este projeto é apenas para fins académicos e não deve ser usado para aplicações comerciais.
