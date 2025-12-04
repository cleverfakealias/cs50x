import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session.get("user_id")
    if request.method == "GET":
        user_rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        if len(user_rows) != 1:
            return apology("User not found.", 400)
        cash = user_rows[0].get("cash")
        # get the current holdings summing shares per symbol
        stock_positions = get_stock_positions(user_id)
        # start building up our holdings by checking the current values of each stock
        holdings = []
        total_holdings = 0
        for row in stock_positions:
            shares = row.get("shares")
            stock_symbol = row.get("symbol")
            current_share_quote = lookup(stock_symbol.upper())
            # these SHOULD BE valid, but in case it can not be located, skip it.
            if current_share_quote is None:
                continue
            stock_price = current_share_quote.get("price")
            current_stock_total = stock_price * shares
            # add the current stock price to the total so we can render on the page
            total_holdings += current_stock_total
            holdings.append(
                # make a new object we can use to render rows on the "/index" route
                {
                    "symbol": stock_symbol,
                    "name": current_share_quote.get("name"),
                    "shares": shares,
                    "price": stock_price,
                    "total": round(current_stock_total, 2)
                }
            )
        return render_template("index.html", holdings=holdings, cash=cash, grand_total=(cash + total_holdings))

    # wasn't valid request method
    return apology("Invalid request", 500)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")
    if request.method == "POST":
        stock_symbol = request.form.get("symbol")
        number_of_shares = request.form.get("shares")
        if not stock_symbol:
            return apology("You must provide a stock symbol (e.g. GOOG).", 400)
        if not number_of_shares:
            return apology("You must provide a number of shares to purchase.", 400)
        # validate the number of shares
        shares = 0
        try:
            shares = int(number_of_shares)
        except ValueError as ex:
            return apology("Number of shares must be valid positive integer.", 400)
        if shares <= 0:
            return apology("Number of shares must be valid positive integer.", 400)
        # get the current price so we can see if user can buy them
        stock_quote = lookup(stock_symbol.upper())
        if stock_quote is None:
            return apology("Invalid stock symbol. Please provide a valid stock.", 400)
        stock_price = stock_quote.get("price")
        # calculate how many the user can buy
        cost = shares * stock_price

        # get the users current information
        user_rows = db.execute(
            "SELECT cash FROM users WHERE id = ?", session.get("user_id"))
        # make sure there is only one
        if len(user_rows) != 1:
            return apology("User not found.", 400)
        user_cash = user_rows[0].get("cash", 0)

        # make sure they can actually afford it
        if user_cash < cost:
            return apology("You are too broke for this many shares!", 400)
        # start update the DB records
        # first record the actual transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session.get("user_id"),
                   stock_quote.get("symbol"),
                   shares,
                   stock_price)
        # next update the users available cash by subtracting the cost from the users current cash
        db.execute("UPDATE users SET cash = (cash - ?) WHERE id = ?",
                   cost,
                   session.get("user_id"))
        return redirect("/")
    # wasn't valid request method
    return apology("Invalid request", 500)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    if request.method == "GET":
        user_id = session.get("user_id")
        user_transactions = db.execute("""
                                       SELECT symbol, shares, price, timestamp
                                       FROM transactions
                                       WHERE user_id = ?
                                       """,
                                       user_id)
        return render_template("history.html", transactions=user_transactions)
    # wasn't valid request method
    return apology("Invalid request", 500)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get(
                "username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.method == "POST":
        # user submitted post request for quote
        stock_symbol = request.form.get("symbol")
        if not stock_symbol:
            return apology("You must provide a stock symbol (e.g. GOOG).", 400)
        # get the current stock price for the symbol
        quote = lookup(stock_symbol.upper())
        if quote is None:
            return apology("Invalid stock symbol. Please provide a valid stock.", 400)
        # render the template with the received quote
        return render_template("quoted.html", quote=quote)
    # wasn't valid request method
    return apology("Invalid request", 500)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    # get the data from the form to create a new user
    if request.method == "POST":
        form = request.form
        username = form.get("username")
        password = form.get("password")
        confirmation = form.get("confirmation")
        # perform the validations on the form fields
        if not username:
            return apology("You must provide a Username.", 400)
        elif not password:
            return apology("You must provide a Password.", 400)
        elif not confirmation:
            return apology("You must confirm the password.", 400)
        elif not password == confirmation:
            return apology("Passwords do not match.", 400)
        # check if we have any existing users with this name
        existing_user_rows = db.execute(
            "SELECT id FROM users WHERE username = ?", username)
        # error if user already taken
        if len(existing_user_rows) > 0:
            return apology("User already taken.", 400)
        # Hash the user password before we store in the database
        password_hash = generate_password_hash(password)
        try:
            new_user_id = db.execute(
                "INSERT INTO users (username, hash) VALUES(?, ?);", username, password_hash)
        except Exception as ex:
            print(ex)
            return apology("An error occurred during registration", 400)
        session["user_id"] = new_user_id
        return redirect("/")
    # wasn't valid request method
    return apology("Invalid request", 500)


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        user_id = session.get("user_id")
        # get the current holdings summing shares per symbol
        stock_positions = get_stock_positions(user_id)
        return render_template("sell.html", stock_positions=stock_positions)
    if request.method == "POST":
        stock_symbol = request.form.get("symbol")
        number_of_shares = request.form.get("shares")
        if not stock_symbol:
            return apology("You must provide a stock symbol (e.g. GOOG).", 400)
        if not number_of_shares:
            return apology("You must provide a number of shares to purchase.", 400)
        number_to_sell = 0
        try:
            number_to_sell = int(number_of_shares)
        except ValueError as ex:
            print(ex)
            return apology("Number of shares must be valid positive integer.", 400)
        if number_to_sell <= 0:
            return apology("Number of shares must be valid positive integer.", 400)
        # get the users current number of shares from the database
        user_stock_shares = db.execute(
            """
                SELECT SUM(shares) AS shares
                FROM transactions
                WHERE user_id = ? and symbol = ?
                GROUP BY symbol
                HAVING SUM(shares) > 0;
                """,
            session.get("user_id"),
            stock_symbol
        )
        if len(user_stock_shares) == 0 or user_stock_shares[0].get("shares") is None:
            return apology(f"You do not own any {stock_symbol.upper()} shares!", 400)
        current_shares = user_stock_shares[0].get("shares")
        if current_shares < number_to_sell:
            return apology(f"Not enough {stock_symbol.upper()} to sell!", 400)
        symbol_quote_price = lookup(stock_symbol)
        if symbol_quote_price is None:
            return apology("Invalid stock symbol. Please provide a valid stock.", 400)
        share_price = symbol_quote_price.get("price")
        proceeds = share_price * number_to_sell

        # insert the new user transation
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            session.get("user_id"), stock_symbol, -number_to_sell, share_price
        )
        # update the users cash
        db.execute(
            "UPDATE users SET cash = (cash + ?) WHERE id = ?", proceeds, session.get("user_id"))
        return redirect("/")
    # wasn't valid request method
    return apology("Invalid request", 500)


def get_stock_positions(user_id):
    try:
        return db.execute(
            """
                SELECT symbol, SUM(shares) AS shares
                FROM transactions
                WHERE user_id = ?
                GROUP BY symbol
                HAVING SUM(shares) > 0;
                """,
            user_id
        )
    except Exception as ex:
        print(ex)
        return None
