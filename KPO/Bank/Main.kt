class BankAccount(val name: String = "Unnamed") {
    private companion object {
        var count = 0
    }

    private var balance = 0
    private val accountNumber = ++count
    fun deposit(sum: Int): Boolean {
        if (sum > 0) {
            balance += sum
            return true
        }
        return false
    }

    fun transfer(account: BankAccount, sum: Int): Boolean {
        if (sum in 1..balance) {
            balance -= sum
            account.balance += sum
            return true
        }
        return false
    }

    override fun toString() = "Account №$accountNumber, name: $name, balance: $balance";
}

class Bank {
    private val accounts = mutableListOf<BankAccount>()

    override fun toString(): String {
        var s = ""
        if (accounts.isEmpty()) {
            return "There are no accounts"
        }
        accounts.forEach { s += it.toString() + "\n" }
        s = s.removeSuffix("\n")
        return s
    }

    fun open(name: String) {
        accounts.add(BankAccount(name))
    }

    fun deposit(number: Int, sum: Int): Boolean {
        return 0 < number && number <= accounts.size && accounts[number - 1].deposit(sum)
    }

    fun transfer(fromAccount: Int, toAccount: Int, sum: Int): Boolean {
        return fromAccount <= accounts.size && toAccount <= accounts.size && accounts[fromAccount - 1].transfer(
            accounts[toAccount - 1],
            sum
        )
    }

    fun size() = accounts.size
}

class Menu(private val bank: Bank) {
    private val menu = """Hello! Enter
            |"1" to see the list of accounts
            |"2" to open an account
            |"3" to make a deposit
            |"4" to transfer money
            |"exit" to finish program
        """.trimMargin()

    fun readCommand(command: String): Boolean {
        var flag = true
        when (command) {
            "1" -> showAccounts()
            "2" -> openAccount()
            "3" -> makeDeposit()
            "4" -> makeTransfer()
            "exit" -> {
                flag = false
                exit()
            }
            else -> println("Incorrect command")
        }
        return flag
    }

    fun showAccounts() {
        println(bank)
    }

    fun openAccount() {
        println("Enter the account name")
        bank.open(readln())
        println("Account is created")
    }

    fun makeDeposit() {
        if (bank.size() < 1) {
            println("Not enough accounts in bank")
            return
        }
        println("Enter the number of account for deposit")
        val n = readln()
        println("Enter the deposit sum")
        val sum = readln()
        if (n.toIntOrNull() == null || sum.toIntOrNull() == null) {
            println("Incorrect input")
            return
        }
        if (!bank.deposit(n.toInt(), sum.toInt())) {
            println("Incorrect data")
        } else {
            println("Deposit is made")
        }
    }

    fun makeTransfer() {
        if (bank.size() < 2) {
            println("Not enough accounts in bank")
            return
        }
        println("Enter the number of account that is being transferred from")
        val fromAccount = readln()
        println("Enter the number of account to which the transfer is being made")
        val toAccount = readln()
        println("Enter the deposit sum")
        val sum = readln()
        if (fromAccount.toIntOrNull() == null || toAccount.toIntOrNull() == null || sum.toIntOrNull() == null) {
            println("Incorrect input")
            return
        }
        if (!bank.transfer(fromAccount.toInt(), toAccount.toInt(), sum.toInt())) {
            println("Incorrect data")
        } else {
            println("Transfer is made")
        }
    }

    fun showMenu() {
        println(menu)
    }

    fun exit() {
        println("Program is finished")
    }

}

fun main() {
    val bank = Bank()
    val menu = Menu(bank)
    var input: String
    var cont: Boolean
    menu.showMenu()
    do {
        input = readln()
        cont = menu.readCommand(input)
        menu.showMenu()
    } while (cont)
}