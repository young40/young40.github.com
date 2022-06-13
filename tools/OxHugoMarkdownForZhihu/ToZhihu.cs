namespace OxHugoMarkdownForZhihu;

public class ToZhihu
{
    public static void ParseFile(string file)
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
        
        /*
        var newLines = new List<string>();

        var remove = false;
        foreach (var line in lines)
        {
            if (line.StartsWith("```plantuml"))
            {
                remove = true;
            }

            if (remove && line.StartsWith("```") && !line.Contains("plantuml"))
            {
                remove = false;
                continue;
            }

            if (remove)
            {
                continue;
            }

            // Console.WriteLine(line);]
            newLines.Add(line);
        }
        */

        File.WriteAllText(file, c);
    }
}