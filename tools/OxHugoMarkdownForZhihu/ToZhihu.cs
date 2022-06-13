namespace OxHugoMarkdownForZhihu;

public class ToZhihu
{
    public static void ParseFile(string file)
    {
        CleanForHugo(file);
        UpdateForZhihu(file);
    }
    
    private static void CleanForHugo(string file)
    {
        if (file.EndsWith(".org"))
        {
            return;
        }

        var c = File.ReadAllText(file);

        c = c.Replace("\\\\(", " $");
        c = c.Replace("\\\\)", "$ ");
        c = c.Replace("\\\\[", "$$");
        c = c.Replace("\\\\]", "\\\\\n$$");
        c = c.Replace("\\\\\\\\", "\\\\\\");

        File.WriteAllText(file, c);
    }

    private static void UpdateForZhihu(string file)
    {
        if (file.EndsWith(".org"))
        {
            return;
        }

        var c = File.ReadAllText(file);
        
        c = c.Replace("\\\\\\", "\\\\");

        string[] lines = c.Split("\n");

        var willSkip = false;
        for (int i = 0; i < lines.Length; i++)
        {
            if (lines[i].StartsWith("#") && lines[i].Contains("{#"))
            {
                var lA = lines[i].Split("{#");
                lines[i] = lA[0];
            }

            if (!lines[i].Trim().StartsWith("$$") && lines[i].Contains("$"))
            {
                lines[i] = lines[i].Replace("$", "$$");
            }

            var link = "https://github.com/young40/young40.github.com/raw/hugo/static";

            link = "http://121.196.158.145";

            if (lines[i].Contains("figure") && lines[i].Contains("/ox-hugo/"))
            {
                var lA = lines[i].Split('"');
                if (lA.Length > 1)
                {
                    lA[1] = lA[1].Replace(".svg", ".png");
                    lines[i] = "<img src=\"" + link + lA[1] + "\" />";
                }
            }

            var hasPlus = lines[i].StartsWith("+++");
            if (hasPlus && !willSkip)
            {
                willSkip = true;
                lines[i] = "";
                continue;
            }

            if (willSkip)
            {
                lines[i] = "";
            }

            if (hasPlus && willSkip)
            {
                willSkip = false;
            }
        }
        

        file = file.Replace("/post/", "/zhihu/");
        Console.WriteLine(file);
        File.WriteAllLines(file, lines);
    }
}