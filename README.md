# Paste
The paste utility writes the textual contents of the Windows clipboard to the console. It is intended to act as acompanion utility to the inbuilt Windows 10 utility, 'clip'.

The 'clip' utility takes output from another process or command and adds it to the clipboard. For some reason, Microsoft does not include the opposite function so here it is, and I have written it specifically to look and feel like the Windows command to allow the two to be used as a pair.

## Installing and running paste
The paste utility is a command line executable and takes the textual contents of the clipboard and simply writes it to the normal output stream. This will either write it to the console, or it can be piped to a file or another process.

Copy the utility to a folder location on the path, or remember to quote the full path when invoking the paste executable.

Run paste with the /? to show some rudimentary help information. Running with any other command line arguments will lead to an error message, as will any other error situations with the utility.

## Examples
Output the textual content of the clipboard to the console:
<pre><code>paste</code></pre>

Write the textual content of the clipboard to a file:
<pre><code>paste &gt; out.txt</code></pre>

Output some help information about the paste utility:
<pre><code>paste /?</code></pre>
