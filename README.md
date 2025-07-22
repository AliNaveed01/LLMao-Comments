# Code Commentor

> An LLM-powered, completely over-engineered 15-minute time-killer  
> that auto-comments your uncommented C code — because who has time to write comments themselves?

## What is this?

You know those moments when you’ve got 15 minutes, a functioning OpenAI key, and a burning desire to automate the obvious?  
This project is exactly that.

**Code Commentor** takes your dull, lifeless C source file and lets a large language model do what you *should’ve* done — explain it with comments.

## How it works (spoiler: with help)

1. Strips your file of existing comments like a judgmental linter.
2. Sends chunks of your code to an LLM (OpenAI API).
3. Reconstructs a *beautifully commented* version of your file.
4. Saves it as `output_commented.c` (because `final_final_REAL.c` was taken).

## Requirements

- A brain (optional, but helpful).
- C compiler (to test your shamefully uncommented code, not this one).
- An OpenAI API key (because we're too cool to write comments ourselves).
- Python (just kidding, it's written in C. Respect.)

## Setup & Running

### 1. Clone it  
Well, unzip it if you're reading this from a zip, or just run:
```bash
git clone <this-repo>
cd code-commentor
````

### 2. Set your OpenAI API Key

Create a `.env` file in the root with this line:

```
OPENAI_API_KEY=your-api-key-here
```

### 3. Build it

```bash
make
```

### 4. Run it

```bash
./commentor test.c
```

This will:

* Read `test.c`
* Comment it using OpenAI
* Output a commented file as `output_commented.c`

### 5. Look Proudly

Open `output_commented.c` and pretend you always had those comments.

## Directory Structure

```
.
├── src/            # The magic (aka: where it happens)
├── include/        # Headers & declarations
├── build/          # Your compiler's mess
├── cJSON/          # Because parsing JSON by hand is for masochists
├── test.c          # Feed this to the beast
├── output_commented.c  # The masterpiece
├── .env            # Your secret sauce
├── Makefile        # Because you’re a real developer
```

## Is this serious?

Absolutely not.
But it works.

## License

If you think this needs a license, you're taking it way too seriously.
