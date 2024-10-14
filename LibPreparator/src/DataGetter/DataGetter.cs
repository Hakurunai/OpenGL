namespace LPrep
{
    internal abstract class DataGetter
    {
#if DEBUG
        protected DebugInfo debugInfo = DebugInfo.All;
#endif
        public abstract Task LoadData(string CurrentDir, string LibDir);
    }
}