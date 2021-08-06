import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStream;
import java.io.ObjectInputStream;
import weka.classifiers.Evaluation;
import weka.classifiers.functions.LibSVM;
import weka.core.Attribute;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.SparseInstance;



public class SVMClassifier {
    LibSVM classifier;
    Instances modelHeader;
    Dataset dataset;

    public SVMClassifier(Dataset dataset, String path) throws Exception {
        System.out.println("_____________________________________________________\n");
        System.out.println("                  Cargando modelo...");

        InputStream is = new BufferedInputStream(new FileInputStream(path));
        ObjectInputStream objectStream = new ObjectInputStream(is);

        classifier = (LibSVM) objectStream.readObject();

        objectStream.close();

        this.dataset = dataset;
        this.modelHeader = dataset.getDataSet();

        
        System.out.println("_____________________________________________________\n");
        System.out.println("                  Evaluacion inicial...");
        Evaluation eval = new Evaluation(modelHeader);
        eval.evaluateModel(classifier, modelHeader);

        System.out.println("_____________________________________________________\n");
        System.out.println("                  Resumen...");
        System.out.println(eval.toSummaryString());
         
        System.out.println("_____________________________________________________\n");
        System.out.println("                  Listo para predecir...");
    }




    public void classifyBatch(String path) throws Exception{
        System.out.println("_____________________________________________________\n");
        System.out.println("                  Batch evaluation...");
        
        
        Instances localData = new Instances(modelHeader);

        BufferedReader reader = new BufferedReader(new FileReader(path));
        String line;
        line = reader.readLine();

        while(line != null)
        {    
            line = line.toLowerCase();
            line = line.replace("target", "");
            line = line.replace("id", "");
            String data[] = line.split(",");
            
            Instance inst = matchInstance(data[2]);
            
            inst.setValue(0, data[1]);
            
            localData.add(inst);
            line = reader.readLine();
        }

        reader.close();

        Evaluation localEval = new Evaluation(localData);
        localEval.evaluateModel(classifier, localData);

        System.out.println(localEval.toSummaryString());

        double[][] CM = localEval.confusionMatrix();
        System.out.println(CM[0][0] + " " + CM[1][0]);
        System.out.println(CM[0][1] + " " + CM[1][1]);
    }




    public boolean classify(String input) throws Exception{
        System.out.println("_____________________________________________________\n");
        System.out.println("                  Realizando predccion...");
        input = input.toLowerCase();
        input = input.replace("target", "");
        input = input.replace("id", "");
        Instance inst = matchInstance(input);

        double result = classifier.classifyInstance(inst);
        double predict[] = classifier.distributionForInstance(inst);
        
        System.out.println("_____________________________________________________\n");
        System.out.println("                  Resultado...");
        System.out.println("Resultado: " + modelHeader.classAttribute().value((int) result));
        System.out.println("Distribucion: " + "Falso: " + predict[0] + " Real: " + predict[1]);
        
        return predict[1] > 0;
    }




    private Instance matchInstance(String input) throws Exception{
        Instances data = dataset.createDummyDataset(input);
        Instance inst = new SparseInstance(modelHeader.numAttributes());
        
        inst.setDataset(modelHeader);

        for(int i = 1; i < modelHeader.numAttributes(); i++){
            inst.setValue(i, 0);
        }
        
        for(int i = 1; i < data.numAttributes(); i++){
            Attribute att = modelHeader.attribute(data.attribute(i).name());
            if (att != null){
                inst.setValue(att.index(), 1);
            }
        }

        inst.setValue(0, "real");

        return inst;
    }
}
