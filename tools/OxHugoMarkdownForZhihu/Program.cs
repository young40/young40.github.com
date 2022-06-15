// See https://aka.ms/new-console-template for more information

using OxHugoMarkdownForZhihu;

Console.WriteLine("Hello, World!");

var file = "/Users/young40/Work/Young40Blog/content/post/2022-06-08-Unity-Rotation-Euler-Quaternion.md";

if (args.Length > 0)
{
    file = args[0];

    if (File.Exists(file))
    {
        Console.WriteLine("Got File: " + file);
        ToZhihu.ParseFile(file);
        return;
    }
    else if (Directory.Exists(file))
    {
        Console.WriteLine("Got Dir: " + file);
        return;
    }
    else
    {
        throw new FileNotFoundException(file);
    }
}

ToZhihu.ParseFile(file);