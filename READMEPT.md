# SOchain - Simulador de Transações com Token SOT

## 📌 Introdução
**SOchain** é uma aplicação em C que simula um sistema de transações com uma criptomoeda chamada **SOT (SO Token)**. Este projeto incorpora conceitos de **Sistemas Operativos**, como gestão de processos, memória partilhada e buffers de comunicação.

Projeto desenvolvido no âmbito da unidade curricular de **Sistemas Operativos**, na Faculdade de Ciências da Universidade de Lisboa.

---

## 📁 Estrutura do Projeto
```
SOCHAIN/
│-- bin/                # Ficheiro executável
│-- inc/                # Ficheiros de cabeçalho (.h)
│-- obj/                # Ficheiros objeto (.o)
│-- src/                # Ficheiros de código-fonte (.c)
│-- makefile            # Script de compilação
│-- README.md           # Este documento
```

---

## 🛠️ Compilação

Comandos disponíveis para compilar e gerir o projeto:

- `make all` – Compila o projeto completo.
- `make` – Compila os ficheiros alterados.
- `make clean` – Remove os ficheiros objeto e executáveis.
- `make re` – Limpa e recompila do zero.

---

## ▶️ Execução

```bash
./bin/SOchain <saldo_inicial> <n_carteiras> <n_servidores> <tam_buffer> <max_tx>
```

### Argumentos:
- `saldo_inicial`: Saldo inicial por carteira (em SOT).
- `n_carteiras`: Número total de carteiras.
- `n_servidores`: Número de servidores de processamento.
- `tam_buffer`: Tamanho do buffer de comunicação.
- `max_tx`: Número máximo de transações a processar.

### Exemplo:
```bash
./bin/SOchain 100 2 2 10 50
```

---

## 💡 Funcionalidades

O programa disponibiliza um menu de comandos:

- `bal <id>` — Ver saldo da carteira com o ID dado.
- `trx <id_origem> <id_destino> <quantia>` — Criar transação entre carteiras.
- `rcp <id>` — Consultar recibo de uma transação.
- `stat` — Mostrar estado atual do sistema.
- `help` — Listar comandos disponíveis.
- `end` — Terminar execução do programa.

---

## 🧩 Módulos Principais

| Módulo       | Função                                               |
|--------------|------------------------------------------------------|
| `main.c/h`   | Criação de processos e interação com o utilizador    |
| `process.c/h`| Gestão de criação e terminação de processos          |
| `memory.c/h` | Gestão de memória dinâmica e partilhada              |
| `wallet.c/h` | Gestão de carteiras e envio de transações            |
| `server.c/h` | Processamento, validação e registo de transações     |

---

## 🧪 Testes

Para efeitos de teste, é disponibilizado um executável de referência (`SOchain_profs`) fornecido pelos docentes.

### Exemplo de teste:
```bash
./SOchain_profs 100 2 2 10 50
```

---

## 👥 Autores

**Grupo 40 - Sistemas Operativos**

- Rodrigo Frutuoso – 61865
- Simão Alexandre – 61874
- Tiago Leite – 61863

---

## 📎 Notas

- Projeto desenvolvido e testado em ambiente **Linux**.
- A sincronização entre processos será abordada na **2.ª fase do projeto**.

---

## ⛔️ Limitações Encontradas
- Quando fazemos uma transação a mensagem "[Main] Introduzir operação: " fica no meio das outras, isso ocorre pela falta de sincronização entre processos.
- Quando se faz só "trx" e depois dermos "end", aparece uma mensagem de erro que os dados da transação são inválidos antes de terminar.

---

## 📄 Licença

Projeto desenvolvido exclusivamente para **fins académicos**.
**Não deve ser utilizado para fins comerciais.**
