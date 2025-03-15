# Projeto1_SO

# SOchain - Simulador de Transações com SOT Token

## Introdução
SOchain é uma aplicação desenvolvida em C para simular um sistema de transações de um token cripto denominado **SOT (SO Token)**. O sistema utiliza conceitos de sistemas operativos, como gestão de processos, memória compartilhada e buffers de comunicação.

Este projeto foi desenvolvido como parte da disciplina de Sistemas Operativos na Faculdade de Ciências da Universidade de Lisboa.

## Estrutura do Projeto
A estrutura do diretório do projeto segue o seguinte padrão:
```
SOCHAIN/
|-- bin/                # Executável do projeto
|-- inc/                # Arquivos de cabeçalho (.h)
|-- obj/                # Arquivos objeto (.o)
|-- src/                # Arquivos fonte (.c)
|-- makefile            # Arquivo de compilação
|-- README.md           # Este documento
```

## Compilação
Para compilar o projeto, basta executar o seguinte comando na raiz do diretório:
```bash
make
```
O executável será gerado no diretório `bin/`.

## Execução
O programa deve ser executado com os seguintes argumentos:
```bash
./bin/SOchain <init_balance> <n_wallets> <n_servers> <buff_size> <max_txs>
```
Onde:
- `init_balance` - Saldo inicial das carteiras.
- `n_wallets` - Número de carteiras no sistema.
- `n_servers` - Número de servidores de processamento.
- `buff_size` - Tamanho máximo dos buffers de comunicação.
- `max_txs` - Número máximo de transações.

## Funcionamento
O programa oferece um menu interativo para gestão das transações:
- `bal <id>` - Consulta o saldo de uma carteira.
- `trx <src_id> <dest_id> <amount>` - Cria uma transação entre carteiras.
- `rcp <id>` - Obtém o comprovativo de uma transação.
- `stat` - Exibe o estado atual do sistema.
- `help` - Mostra a lista de comandos disponíveis.
- `end` - Finaliza a execução do SOchain.

## Módulos Principais
- `main.c/h` - Gera os processos e interage com o usuário.
- `process.c/h` - Gerencia a criação e destruição de processos.
- `memory.c/h` - Aloca e gerencia a memória compartilhada.
- `wallet.c/h` - Assina transações e as encaminha para os servidores.
- `server.c/h` - Valida, processa e registra as transações.

## Testes
Para testar o programa, utilize o executável disponibilizado pelos professores `SOchain_profs` e compare os resultados.

Exemplo de teste:
```bash
./SOchain 100 2 2 10 50
```
Isso inicia o sistema com saldo inicial de 100 SOT, 2 carteiras, 2 servidores, buffers de tamanho 10 e um máximo de 50 transações.

## Autores
- Nome 1 (Número do aluno)
- Nome 2 (Número do aluno)

## Observações
- O projeto foi desenvolvido e testado em um ambiente Linux.
- O uso de sincronização entre processos será abordado na segunda fase do projeto.

## Licença
Este projeto é apenas para fins acadêmicos e não deve ser utilizado para propósitos comerciais.

