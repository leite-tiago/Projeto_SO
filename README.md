# SOchain - Transaction Simulator with SOT Token

## Introduction
SOchain is a C-based application designed to simulate a transaction system for a cryptocurrency token called **SOT (SO Token)**. The system incorporates operating system concepts such as process management, shared memory, process synchronization (with semaphores), signals, alarms, and file I/O.

This project was developed as part of the Operating Systems course at the Faculty of Sciences, University of Lisbon.

## Project Structure
The project follows this directory structure:
```
SOCHAIN/
|-- bin/                # Executable files
|-- inc/                # Header files (.h)
|-- obj/                # Object files (.o)
|-- src/                # Source code files (.c)
|-- makefile            # Compilation script
|-- README.md           # This document
|-- args.txt            # Input arguments file
|-- settings.txt        # Settings file (log, stats, period)
```

## Compilation
To manage the project, use the following commands:
- **Compile the project**: `make all`
- **Compile only**: `make`
- **Clean compiled files**: `make clean`
- **Recompile from scratch**: `make re`

## Execution

**Phase 2:**
Run the program using:
```bash
./bin/SOchain args.txt settings.txt
```
Where:
- `args.txt` contains, line by line:
    1. Initial balance for each wallet (`init_balance`)
    2. Number of wallets (`n_wallets`)
    3. Number of servers (`n_servers`)
    4. Buffer size (`buff_size`)
    5. Maximum number of transactions (`max_txs`)
- `settings.txt` contains, line by line:
    1. Log file name (e.g., `log.txt`)
    2. Statistics file name (e.g., `stats.txt`)
    3. Period for periodic statistics print (in seconds, use `0` to disable)

**Example `args.txt`:**
```
100.0
2
2
10
50
```
**Example `settings.txt`:**
```
log.txt
stats.txt
5
```

### Example Run
```bash
./bin/SOchain args.txt settings.txt
```

## Functionality

The program provides an interactive menu for managing transactions:
- `bal <id>`: Check the balance of a specific wallet.
- `trx <src_id> <dest_id> <amount>`: Create a transaction from one wallet to another.
- `rcp <id>`: Retrieve the receipt of a transaction.
- `stat`: Display the current state of the system.
- `help`: Show the list of available commands.
- `end`: Terminate the SOchain execution.

### New Features in Phase 2
- **Synchronization:** All access to shared memory is protected by semaphores (producer/consumer model).
- **File Input:** Arguments are read from `args.txt` and settings from `settings.txt`.
- **Logging:** All user operations are logged to the file specified in `settings.txt` with timestamps.
- **Periodic Statistics:** If the period in `settings.txt` is greater than 0, the system prints the status of all transactions periodically.
- **Signal Handling:** The program handles `SIGINT` (CTRL+C) to terminate gracefully, releasing all resources.
- **Statistics File:** On termination, final statistics are written to the file specified in `settings.txt`.

## Main Modules
- `main.c/h`: Manages process creation and user interaction.
- `process.c/h`: Handles process creation and termination.
- `memory.c/h`: Manages dynamic and shared memory allocation.
- `wallet.c/h`: Signs transactions and forwards them to servers.
- `server.c/h`: Validates, processes, and records transactions.
- `synchronization.c/h`: Implements semaphores for synchronization.
- `clog.c/h`: Handles logging of user operations.
- `csettings.c/h`: Reads configuration files.
- `ctime.c/h`: Manages timestamps for transactions.
- `csignal.c/h`: Handles signals and alarms.
- `cstats.c/h`: Handles statistics output.

## Testing

To test the program:
1. **Compile:**
   ```bash
   make
   ```
2. **Prepare input files:**
   Edit `src/args.txt` and `src/settings.txt` as shown above.
3. **Run:**
   ```bash
   ./bin/SOchain src/args.txt src/settings.txt
   ```
4. **Interact:**
   Use the commands (`bal`, `trx`, `rcp`, `stat`, `help`, `end`) as described above.
5. **Check outputs:**
   - Log file (e.g., `log.txt`) for operation history.
   - Statistics file (e.g., `stats.txt`) for final stats.
   - Periodic statistics on the terminal if enabled.
6. **Test signals:**
   Press `CTRL+C` to ensure the program terminates gracefully and writes statistics.

## Authors
**Grupo 40**
- Rodrigo Frutuoso - 61865
- Simão Alexandre - 61874
- Tiago Leite - 61863

## Notes
- The project was developed and tested in a Linux environment.
- All synchronization, logging, and signal handling are implemented in this phase.

## License
This project is for academic purposes only and should not be used for commercial applications.
