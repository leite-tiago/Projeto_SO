# SOchain - Transaction Simulator with SOT Token

## Introduction
SOchain is a C-based application designed to simulate a transaction system for a cryptocurrency token called **SOT (SO Token)**. The system incorporates operating system concepts such as process management, shared memory, and communication buffers.

This project was developed as part of the Operating Systems course at the Faculty of Sciences, University of Lisbon.

## Project Structure
The project follows this directory structure:
```
SOCHAIN/
|-- bin/                # Executable file
|-- inc/                # Header files (.h)
|-- obj/                # Object files (.o)
|-- src/                # Source code files (.c)
|-- makefile            # Compilation script
|-- README.md           # This document
```

## Compilation
To compile the project, run the following command in the root directory:
```bash
make
```
The executable will be generated in the `bin/` directory.

## Execution
Run the program using the following command:
```bash
./bin/SOchain <init_balance> <n_wallets> <n_servers> <buff_size> <max_txs>
```
Where:
- `init_balance` - Initial balance for each wallet.
- `n_wallets` - Number of wallets in the system.
- `n_servers` - Number of transaction processing servers.
- `buff_size` - Maximum buffer size (i.e., the maximum number of transactions in communication buffers at a time).
- `max_txs` - Maximum number of transactions the system can process.

## Functionality
The program provides an interactive menu for managing transactions:
- `bal <id>` - Check the balance of a specific wallet.
- `trx <src_id> <dest_id> <amount>` - Create a transaction from one wallet to another.
- `rcp <id>` - Retrieve the receipt of a transaction.
- `stat` - Display the current state of the system.
- `help` - Show the list of available commands.
- `end` - Terminate the SOchain execution.

## Main Modules
- `main.c/h` - Manages process creation and user interaction.
- `process.c/h` - Handles process creation and termination.
- `memory.c/h` - Manages dynamic and shared memory allocation.
- `wallet.c/h` - Signs transactions and forwards them to servers.
- `server.c/h` - Validates, processes, and records transactions.

## Testing
To test the program, use the reference executable `SOchain_profs` provided by the instructors and compare the results.

Example test:
```bash
./SOchain 100 2 2 10 50
```
This initializes the system with a balance of 100 SOT per wallet, 2 wallets, 2 servers, a buffer size of 10, and a maximum of 50 transactions.

## Authors
- Tiago Leite
- Rodrigo Frutuoso
- Simão Alexandre

## Notes
- The project was developed and tested in a Linux environment.
- Process synchronization will be implemented in the second phase of the project.

## License
This project is for academic purposes only and should not be used for commercial applications.

