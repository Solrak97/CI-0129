import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import weka.core.stemmers.NullStemmer;
import weka.core.tokenizers.NGramTokenizer;
import weka.filters.Filter;
import weka.filters.unsupervised.attribute.NominalToString;
import weka.filters.unsupervised.attribute.Remove;
import weka.filters.unsupervised.attribute.StringToWordVector;
import weka.core.Attribute;
import weka.core.Instance;

//arrf creation
import weka.core.Instances;
import weka.core.SparseInstance;
import weka.core.converters.ArffSaver;
import weka.core.converters.ConverterUtils.DataSource;


public class Dataset {
    Instances dataset;
    
    public Dataset(String path) throws Exception{
        DataSource source = new DataSource(path);
        dataset = source.getDataSet();
    }

    public Dataset(){
    }



    public Filter createParametrizedNominal2String(){
        String[] options = {"-C", "3"};
        Filter n2s = new NominalToString();
        try {
            n2s.setOptions(options);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return n2s;
    }

    public Filter createParametrizedString2WordVector(){
        String[] tokenOptions = {"-max", "3", "-min", "1", "-delimiters", " \r \t.,;:\'\"()?!_"};
        String[] s2wvOptions = {"-R", "3", "-W", "1000", "-prune-rate", "1.0", "-L"};
        
        NullStemmer stemmer = new NullStemmer();

        NGramTokenizer tokenizer = new NGramTokenizer();
        try {
            tokenizer.setOptions(tokenOptions);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        StringToWordVector s2wv = new StringToWordVector();
        try {
            s2wv.setOptions(s2wvOptions);
        } catch (Exception e) {
            e.printStackTrace();
        }

        
        s2wv.setStemmer(stemmer);
        s2wv.setTokenizer(tokenizer);
        
        return s2wv;
    }

    public Remove removeFilter(){
        Remove removeFilter = new Remove();
        removeFilter.setAttributeIndices("1");
        return removeFilter;
    }

    public void applyFilters() throws Exception{
        Filter n2s = createParametrizedNominal2String();
        Filter s2wv = createParametrizedString2WordVector();
        Filter remove = removeFilter();
        n2s.setInputFormat(dataset);
        dataset = Filter.useFilter(dataset, n2s);
        s2wv.setInputFormat(dataset);
        dataset = Filter.useFilter(dataset, s2wv);
        remove.setInputFormat(dataset);
        dataset = Filter.useFilter(dataset, remove);
    }

    public void load(String path) throws Exception{
        DataSource source = new DataSource(path);
        dataset = source.getDataSet();
        dataset.setClassIndex(1);
        for(int i = 0; i < dataset.numAttributes(); i++){
            
        dataset.setAttributeWeight(i, 1);
        }
    }

    public void save(String path) throws IOException{
        ArffSaver saver = new ArffSaver();
        saver.setInstances(dataset);
        saver.setFile(new File(path));
        saver.writeBatch();
    }

    public Instances getDataSet(){
        return dataset;
    }

    public Instances createDummyDataset(String inputText) throws Exception{
      //  Set ID
      Attribute id = new Attribute("id");
        
      //  Set classes
      ArrayList<String> nominal = new ArrayList<String>(3);
      nominal.add("fake");
      nominal.add("real");
      Attribute target = new Attribute("target", nominal);

      //  Set nominal text
      ArrayList<String> nominaltext = new ArrayList<String>(1);
      nominaltext.add(inputText);
      Attribute text = new Attribute("text", nominaltext);
      
      //Set attributes
      ArrayList<Attribute> attr = new ArrayList<Attribute>();
      attr.add(id);
      attr.add(target);
      attr.add(text);
      Instances tempDataset = new Instances("data", attr, 0);
      tempDataset.setClassIndex(1);
      
      //    Set Instance
      Instance inst = new SparseInstance(3);
      inst.setValue(id, 1);
      inst.setValue(target, "fake");
      inst.setValue(text, inputText);
      inst.setDataset(dataset);
      tempDataset.add(inst);

      Filter n2s = createParametrizedNominal2String();
      Filter s2wv = createParametrizedString2WordVector();
      n2s.setInputFormat(tempDataset);
      tempDataset = Filter.useFilter(tempDataset, n2s);
      
      s2wv.setInputFormat(tempDataset);
      tempDataset = Filter.useFilter(tempDataset, s2wv);
      
      Remove removeFilter = removeFilter();
      removeFilter.setInputFormat(tempDataset);

      tempDataset = Filter.useFilter(tempDataset, removeFilter);
      return tempDataset;    
    }
}
